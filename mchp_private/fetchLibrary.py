#!/bin/python3

import argparse, os, sys, traceback, subprocess
import shutil, glob

NORMAL  = "\x1B""[0m"
RED     = "\x1B""[1;31m"
GREEN   = "\x1B""[1;32m"
YELLOW  = "\x1B""[1;33m"
CYAN    = "\x1B""[1;36m"

# Default library path root
LIBRARY_REPOSITORY_ROOT = os.path.join('fmpp', 'build', 'lib')

# Source library root path.
LIBRARIES_ROOT = "lib"

LIBRARY_NAME_PATTERN = 'libcam*-dspic33a.a'

# Default CAM repo name and path.
CRYPTO_CAM_LIBRARY_REPOSITORY_NAME = 'msec-driver-hsm-lite-cam'
CRYPTO_CAM_LIBRARY_REPOSITORY_DEFAULT_PATH = os.path.join(LIBRARY_REPOSITORY_ROOT, CRYPTO_CAM_LIBRARY_REPOSITORY_NAME)
CRYPTO_CAM_LIBRARY_DEFAULT_GIT = 'https://bitbucket.microchip.com/scm/msec/msec-driver-hsm-lite-cam.git'

LIB_ROOT = os.path.join('..', 'lib')

# Header files to copy for each library.
LIB_HEADERS = ['version', 'hash', 'aes', 'ecdsa', 'ecdh', 'pke', 'trng', 'device']

SHOW_COMMANDS = True


# Run commands as a set.
def run_commands(cmds, capture_output=False, cwd=None):
    '''
        Run a set of commands, provided as a list.
    '''
    captured_output=[]
    for c in cmds:
        try:
            # If the command is already a list, use it as-is.
            if type(c) is list:
                cmd = c
            else:
                cmd = c.split(' ')

            if SHOW_COMMANDS is True:
                print(f"\nRunning command '{' '.join(cmd)}'")

            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, cwd=cwd)

            # Pull output until done.
            while True:
                output = proc.stdout.readline()
                if proc.poll() is not None:
                    break

                # Print and log the output.
                if output:
                    if capture_output is True:
                        captured_output.append(output)
                    else:
                        print(output.strip().decode("utf-8"))

            # Get the return code...
            retcode = proc.poll()
            if retcode:
                raise Exception(f"Command '{' '.join(cmd)}' failed with result code {retcode}")

        except OSError:
            raise Exception(f"Failed to start execution of command '{' '.join(cmd)}'")

    if True == capture_output:
        return captured_output
    else:
        return None

# Extract the IP name from a library filename (e.g., 'libcam05346-dspic33a.a' -> 'cam_05346').
def ip_name_from_library(libname: str) -> str:
    # Expected format: libcam<NNNNN>-dspic33a.a
    import re
    m = re.match(r'libcam(\d+)-dspic33a\.a', libname)
    if m:
        return 'cam_%s' % m.group(1)
    return None


# Copy the build files to the target location
def install(sourcepath):
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        srcdir = os.path.join(curdir, sourcepath, LIBRARIES_ROOT)

        print(YELLOW + f"\nCopying files from '{sourcepath}'..." + NORMAL)

        # Discover library files matching the expected naming pattern.
        libfiles = glob.glob(os.path.join(srcdir, LIBRARY_NAME_PATTERN))
        if len(libfiles) == 0:
            print(RED + f"  *** No library files matching '{LIBRARY_NAME_PATTERN}' found in '{srcdir}'" + NORMAL)
            return False

        # Short term workaround: if there are no header files in the source directory, we need to add
        # a path to the header files in the repo and copy from there.
        headersrcdir = srcdir
        headerfiles = glob.glob(os.path.join(srcdir, '*.h'))
        if len(list(headerfiles)) == 0:
            print(RED + "  *** No header files in libraries folder - copying from repo location" + NORMAL)
            headersrcdir = os.path.join(curdir, sourcepath, 'inc')

        # Install each discovered library into its per-IP subdirectory.
        for libfile in libfiles:
            libname = os.path.basename(libfile)
            ip_name = ip_name_from_library(libname)
            if ip_name is None:
                print(RED + f"  *** Unable to determine IP name from '{libname}'" + NORMAL)
                result = False
                continue

            dstdir = os.path.join(curdir, LIB_ROOT, ip_name, 'crypto', 'drivers', 'library')
            if not os.path.exists(dstdir):
                print(f"  Creating target directory '{dstdir}'...")
                os.makedirs(dstdir)

            FILES = [(os.path.join(srcdir, libname), os.path.join(dstdir, libname))]

            for p in LIB_HEADERS:
                headerfile = 'cam_%s.h' % p
                FILES.append((os.path.join(headersrcdir, headerfile), os.path.join(dstdir, headerfile)))

            print(YELLOW + f"\n  Installing {libname} -> {ip_name}/library/" + NORMAL)
            skipped = 0
            for f in FILES:
                src, dest = f
                if not os.path.exists(src):
                    print(YELLOW + "    [skipped] %s (not found)" % os.path.basename(src) + NORMAL)
                    skipped += 1
                    continue
                print(CYAN + "    %s -> %s" % (os.path.basename(src), dest.replace(curdir + os.sep, '')) + NORMAL)
                shutil.copy(src, dest)

            if skipped > 0:
                print(YELLOW + f"  Warning: {skipped} file(s) not found and could not be copied." + NORMAL)

        print(" ")

    except Exception as ex:
        print(RED + f"\nFailed to copy files: {str(ex)}" + NORMAL)
        result = False

    return result

# Clone / update the repository.
def update_clone(options):
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        repodir = os.path.join(curdir, options.repository)
        if not os.path.exists(repodir):
            print(YELLOW + f"\nCloning {CRYPTO_CAM_LIBRARY_REPOSITORY_NAME} repository at '{repodir}'..." + NORMAL)
            CLONE_CMD = [f"git clone {options.git} {options.repository}"]
            run_commands(CLONE_CMD)

        print(YELLOW + f"\nUpdating repository at '{repodir}'..." + NORMAL)
        if options.branch is not None:
            GIT_CMDS = ["git fetch",
                        f"git checkout {options.branch}",
                        "git pull"]

            run_commands(GIT_CMDS, cwd=repodir)
        if options.tag is not None:
            GIT_CMDS = ["git fetch",
                        f"git checkout {options.tag}"]

            run_commands(GIT_CMDS, cwd=repodir)
    except Exception as ex:
        print(RED + f"\nFailed to clone repository: {str(ex)}" + NORMAL)
        result = False

    return result


# Main
if __name__ == "__main__":

    # Defined this class to support verbose help on argument error.
    class MyParser(argparse.ArgumentParser):
        def error(self, message):
            sys.stderr.write(RED + f"error: {message}\n" + NORMAL)
            self.print_help()
            sys.exit(2)

    try:
        result = False

        # Specify arguments.
        parser = MyParser(description=__doc__)

        parser.add_argument('-r', '--repository', help='use this pre-existing CAM library repository', type=str)
        parser.add_argument('-c', '--clone', help='attempt to clone/update the CAM library repository', action="store_true")
        parser.add_argument('-g', '--git', help='fetch the CAM library repository from this Git link', type=str)
        parser.add_argument('-b', '--branch', help='if cloning/updating, branch name for checkout', type=str)
        parser.add_argument('-i', '--install', help='install libraries from pre-existing library directory', type=str)
        parser.add_argument('-t', '--tag', help='if cloning/updating, tag or commit hash value for checkout', type=str)

        options = parser.parse_args()

        if options.install is None and options.repository is None:
            if options.clone is False:
                parser.error("Must specify a pre-existing repository path or a pre-existing library path")

        if options.install is not None:
            if options.repository is not None:
                parser.error("cannot specify an install path with a pre-existing repository")

            elif not os.path.exists(options.install):
                parser.error(f"Library source directory '{options.install}' not found.")

        elif options.repository is not None:
            if options.git is not None:
                parser.error("Cannot use a pre-existing repository and specify a repository to clone")

            if not os.path.exists(os.path.join(options.repository, '.git')):
                parser.error(f"Unable to locate repository at '{options.repository}'")
        else:
            options.repository = CRYPTO_CAM_LIBRARY_REPOSITORY_DEFAULT_PATH

        if options.git is not None:
            if options.clone is False:
                parser.error("Cannot specify a repository to clone without also asking to clone it")
        elif options.clone is True:
            options.git = CRYPTO_CAM_LIBRARY_DEFAULT_GIT

        # Clone the repo if requested.
        if options.clone is True:
            if options.branch is None and options.tag is None:
                parser.error("Must specify a branch or tag name if cloning the repository.")
            elif options.branch is not None and options.tag is not None:
                parser.error("Must specify a branch or tag name not both.")

            result = update_clone(options)
            if result is False:
                sys.exit(1)

        # Install the library files.
        if options.repository is not None:
            result = install(options.repository)

        elif options.install is not None:
            result = install(options.install)

        if result is True:
            print(GREEN + "Copy complete." + NORMAL)
            sys.exit(0)
        else:
            print(RED + "Copy failed!" + NORMAL)
            sys.exit(1)

    except Exception as ex:
        print(RED + f"Copy failed: {str(ex)}" + NORMAL)
        print(traceback.format_exc())

    sys.exit(1)
