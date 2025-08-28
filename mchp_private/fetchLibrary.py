#!/bin/python3

import argparse, os, sys, traceback, subprocess
import shutil, glob

NORMAL  = "\x1B""[0m"
RED     = "\x1B""[1;31m"
GREEN   = "\x1B""[1;32m"
YELLOW  = "\x1B""[1;33m"
BLUE    = "\x1B""[1;34m"
MAGENTA = "\x1B""[1;35m"
CYAN    = "\x1B""[1;36m"

# Default library path root
LIBRARY_REPOSITORY_ROOT = os.path.join('lib')

# Source library root path.
LIBRARIES_ROOT = "libraries"

INCLUDE_ROOT = os.path.join("driver", "include")

LIBRARY_NAME = 'libcam05346-dspic33a.a'

# Default CAM repo name and path.
CRYPTO_CAM_LIBRARY_REPOSITORY_NAME = 'crypto_cam_src'
CRYPTO_CAM_LIBRARY_REPOSITORY_DEFAULT_PATH = os.path.join(LIBRARY_REPOSITORY_ROOT, CRYPTO_CAM_LIBRARY_REPOSITORY_NAME)
CRYPTO_CAM_LIBRARY_DEFAULT_GIT = 'https://bitbucket.microchip.com/scm/mh3/%s.git' % CRYPTO_CAM_LIBRARY_REPOSITORY_NAME

APPS_ROOT = os.path.join('..', 'dspic33ak512mps512')
CRYPTO_LIBRARY_PATH = os.path.join('crypto', 'drivers', 'library')

HASH_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'hash', 'sha.X', CRYPTO_LIBRARY_PATH)
AES_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'aes', 'aes.X', CRYPTO_LIBRARY_PATH)
ECDSA_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'dsa', 'ecdsa.X', CRYPTO_LIBRARY_PATH)
ECDH_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'ecdh', 'ecdh.X', CRYPTO_LIBRARY_PATH)
TRNG_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'trng', 'trng.X', CRYPTO_LIBRARY_PATH)
AES_HASH_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'aes_hash', 'aes_hash.X', CRYPTO_LIBRARY_PATH)

# Header files associated with each project.
LIB_COPIES = [('sha', HASH_CRYPTO_LIBDIR, ['version', 'hash']),
              ('aes', AES_CRYPTO_LIBDIR, ['version', 'aes']),
              ('dsa', ECDSA_CRYPTO_LIBDIR, ['version', 'ecdsa', 'pke']),
              ('ecdh', ECDH_CRYPTO_LIBDIR, ['version', 'ecdh', 'pke']),
              ('trng', TRNG_CRYPTO_LIBDIR, ['version', 'trng']),
              ('aes_hash', AES_HASH_CRYPTO_LIBDIR, ['version', 'hash', 'aes']),
             ]

SHOW_COMMANDS = True


# Run commands as a set.
def run_commands(cmds, capture_output=False):
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

            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)

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

# Copy the build files to the target location
def install(sourcepath):
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        print(YELLOW + f"\nCopying files from '{sourcepath}'..." + NORMAL)
        for c in LIB_COPIES:
            project, dstdir, path = c

            srcdir = os.path.join(curdir, sourcepath, LIBRARIES_ROOT)
            dstdir = os.path.join(curdir, dstdir)

            FILES = [(os.path.join(srcdir, LIBRARY_NAME), os.path.join(dstdir, LIBRARY_NAME))]

            print(YELLOW + f"\nCopying files for project '{project}'..." + NORMAL)

            if not os.path.exists(dstdir):
                print(f"  Creating target directory '{dstdir}'...")
                os.makedirs(dstdir)

            # Short term workaround: if there are no header files in the source directory, we need to add
            # a path to the header files in the repo and copy from there.
            headerfiles = glob.glob(os.path.join(srcdir, '*.h'))
            if len(list(headerfiles)) == 0:
                print(RED + "  *** No header files in libraries folder - copying from repo location" + NORMAL)
                srcdir = os.path.join(curdir, sourcepath, 'driver', 'include')

            for p in path:
                headerfile = 'cam_%s.h' % p
                FILES.append((os.path.join(srcdir, headerfile), os.path.join(dstdir, headerfile)))

            for f in FILES:
                src, dest = f
                print("  %s -> %s" % (src.replace(os.path.join(curdir, sourcepath) + os.sep, ''), dest.replace(curdir + os.sep, '')))
                shutil.copy(src, dest)

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

        os.chdir(repodir)

        print(YELLOW + f"\nUpdating repository at '{repodir}'..." + NORMAL)
        GIT_CMDS = ["git fetch",
                    f"git checkout {options.branch}",
                    "git pull"]

        run_commands(GIT_CMDS)

    except Exception as ex:
        print(RED + f"\nFailed to clone repository: {str(ex)}" + NORMAL)
        result = False

    os.chdir(curdir)
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
        parser.add_argument('-g', '--git', help='fetch the CAM library repository from this Git link', type=str, default=CRYPTO_CAM_LIBRARY_DEFAULT_GIT)
        parser.add_argument('-b', '--branch', help='if cloning/updating, branch name for checkout', type=str)
        parser.add_argument('-i', '--install', help='install libraries from pre-existing library directory', type=str)

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
            if options.clone is None:
                parser.error("Cannot specify a repository to clone without also asking to clone it")

        # Clone the repo if requested.
        if options.clone is True:
            if options.branch is None:
                parser.error("Must specify a branch name if cloning the repository.")

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
