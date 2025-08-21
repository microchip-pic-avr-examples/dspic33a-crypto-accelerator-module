#!/bin/python3

import argparse, os, sys, traceback, subprocess
import shutil

NORMAL  = "\x1B""[0m"
RED     = "\x1B""[1;31m"
GREEN   = "\x1B""[1;32m"
YELLOW  = "\x1B""[1;33m"
BLUE    = "\x1B""[1;34m"
MAGENTA = "\x1B""[1;35m"
CYAN    = "\x1B""[1;36m"

FMPP_ROOT = 'fmpp'
BUILD_ROOT = os.path.join(FMPP_ROOT, 'build')

INPUT_ROOT = os.path.join(BUILD_ROOT, 'input')
COMMON_CRYPTO_ROOT = os.path.join(INPUT_ROOT, 'common_crypto')
TEMPLATE_ROOT = os.path.join(INPUT_ROOT, 'templates')

OUTPUT_ROOT = os.path.join(BUILD_ROOT, 'output')

APPS_ROOT = os.path.join('..', 'dspic33ak512mps512')

CRYPTO_V4_REPOSITORY_NAME = 'crypto_v4'
CRYPTO_V4_REPOSITORY_DEFAULT_PATH = os.path.join(BUILD_ROOT, CRYPTO_V4_REPOSITORY_NAME)
CRYPTO_V4_REPOSITORY_DEFAULT_GIT = 'https://bitbucket.microchip.com/scm/mh3/%s.git' % CRYPTO_V4_REPOSITORY_NAME

# Common target crypto firmware path.
CRYPTO_FIRMWARE_PATH = os.path.join('crypto')


HASH_CRYPTO_DIR = os.path.join(APPS_ROOT, 'hash', 'sha.X', CRYPTO_FIRMWARE_PATH)
AES_CRYPTO_DIR = os.path.join(APPS_ROOT, 'aes', 'aes.X', CRYPTO_FIRMWARE_PATH)
ECDSA_CRYPTO_DIR = os.path.join(APPS_ROOT, 'dsa', 'ecdsa.X', CRYPTO_FIRMWARE_PATH)
ECDH_CRYPTO_DIR = os.path.join(APPS_ROOT, 'ecdh', 'ecdh.X', CRYPTO_FIRMWARE_PATH)
TRNG_CRYPTO_DIR = os.path.join(APPS_ROOT, 'trng', 'trng.X', CRYPTO_FIRMWARE_PATH)
AES_HASH_CRYPTO_DIR = os.path.join(APPS_ROOT, 'aes_hash', 'aes_hash.X', CRYPTO_FIRMWARE_PATH)

# Format of each entry is a quadruple: (project-name, target-dir, [list-of-common-crypto-files], [list-of-cam-files])
FILE_COPIES = [('hash', HASH_CRYPTO_DIR, ['hash'], ['hash_cam', 'cam']),
               ('aes', AES_CRYPTO_DIR, ['sym_cipher', 'mac_cipher', 'aead_cipher'], ['sym_cam', 'mac_cam', 'aead_cam', 'cam']),
               ('dsa', ECDSA_CRYPTO_DIR, ['digsign'], ['digisign_cam', 'cam']),
               ('ecdh', ECDH_CRYPTO_DIR, ['kas'], ['kas_cam', 'cam']),
               ('trng', TRNG_CRYPTO_DIR, ['rng'], ['rng_cam', 'cam']),
               ('aes+hash', AES_HASH_CRYPTO_DIR, ['hash', 'sym_cipher', 'mac_cipher', 'aead_cipher'], ['hash_cam', 'sym_cam', 'mac_cam', 'aead_cam', 'cam'])]

# Set to show the commands being executed.
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


# Check if the given IP is present in the repository.
def check_for_ip(repodir, ipname):
    result = True

    ipdir = os.path.join(repodir, 'module_crypto', 'src', 'drivers', ipname)
    if not os.path.exists(ipdir):
        print(RED + f"\nIP '{ipname}' does not exist in the repository at '{repodir}'." + NORMAL)
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
            print(YELLOW + f"\nCloning {CRYPTO_V4_REPOSITORY_NAME} repository at '{repodir}'..." + NORMAL)
            CLONE_CMD = [f"git clone {options.git} {options.repository}"]
            run_commands(CLONE_CMD)

        os.chdir(repodir)

        print(YELLOW + f"\nUpdating repository at '{repodir}'..." + NORMAL)
        GIT_CMDS = ['git fetch',
                    'git checkout %s' % options.branch,
                    'git pull']

        run_commands(GIT_CMDS)

        result = check_for_ip(repodir, options.ipname)

    except Exception as ex:
        print(RED + f"\nFailed to clone repository: {str(ex)}" + NORMAL)
        result = False

    os.chdir(curdir)
    return result


# Create all directories.
def create_directories(options):
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        print(YELLOW + "\nCreating directories..." + NORMAL)

        inputdir = os.path.join(curdir, INPUT_ROOT)
        ipdir = os.path.join(curdir, inputdir, options.ipname)
        ccdir = os.path.join(curdir, COMMON_CRYPTO_ROOT)
        templatedir = os.path.join(curdir, TEMPLATE_ROOT)
        outputdir = os.path.join(curdir, OUTPUT_ROOT)

        for d in [inputdir, ipdir, ccdir, templatedir, outputdir]:
            if not os.path.exists(d):
                print("  Creating directory '%s'..." % d)
                os.makedirs(d)

    except Exception as ex:
        print(RED + f"\nFailed to create directory: {str(ex)}" + NORMAL)
        result = False

    return result


# Copy and generate the output files.
def generate_files(options):
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        ipsrcdir = os.path.join(curdir, options.repository, 'module_crypto', 'src', 'drivers', options.ipname)
        ipdstdir = os.path.join(curdir, INPUT_ROOT, options.ipname)

        ccsrcdir = os.path.join(curdir, options.repository, 'module_crypto', 'src', 'common_crypto')
        ccdstdir = os.path.join(curdir, COMMON_CRYPTO_ROOT)

        templatesrcdir = os.path.join(curdir, options.repository, 'module_crypto', 'templates')
        templatedstdir = os.path.join(curdir, TEMPLATE_ROOT)

        FMPP_COPIES = [(ipsrcdir, ipdstdir), (ccsrcdir, ccdstdir), (templatesrcdir, templatedstdir)]

        print(YELLOW + "\nCopying source FTL files..." + NORMAL)
        for c in FMPP_COPIES:
            try:
                srcdir, dstdir = c
                if not os.path.exists(srcdir):
                    print(RED + f"Could not locate source FTL files at '{srcdir}'" + NORMAL)
                    result = False
                    break

                if os.path.exists(dstdir):
                    shutil.rmtree(dstdir)

                print("  %s -> %s" % (srcdir.replace(curdir + os.sep, ''), dstdir.replace(curdir + os.sep, '')))
                shutil.copytree(srcdir, dstdir)

            except Exception as ex:
                print(RED + f"Failed to copy files from '{srcdir}' to '{dstdir}': {str(ex)}" + NORMAL)
                print(traceback.format_exc())
                result = False
                break

        # Run FMPP to generate the files.
        if result is True:
            print(YELLOW + "\nGenerating files..." + NORMAL)
            FMPP_CMD = [['fmpp',
                        '-C', os.path.join(curdir, FMPP_ROOT, 'config.fmpp'),
                        '-S', os.path.join(curdir, INPUT_ROOT),
                        '-O', os.path.join(curdir, OUTPUT_ROOT)]
                    ]

            run_commands(FMPP_CMD)

    except Exception as ex:
        print(RED + f"\nFailed to clone repository: {str(ex)}" + NORMAL)
        result = False

    return result


# Copy the generated files to their target.
def copy_generated_files(options):
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        print(YELLOW + "\nCopying output files..." + NORMAL)

        try:
            _, ipvalue = options.ipname.split('_', 1)

        except Exception as ex:
            print(RED + f"Unable to determine IP value from '{options.ipname}': {str(ex)}" + NORMAL)
            print(traceback.format_exc())
            return False

        for c in FILE_COPIES:
            project, dir, p1, p2 = c

            CRYPTO_DIR = os.path.join(dir, 'common_crypto')
            CRYPTO_SDIR = os.path.join(dir, 'common_crypto', 'src')

            DRIVERS_DIR = os.path.join(dir, 'drivers', 'wrapper')
            DRIVERS_SDIR = os.path.join(dir, 'drivers', 'wrapper', 'src')

            print(YELLOW + f"\nCopying files for '{project}'..." + NORMAL)
            print(YELLOW + f"\n  Processing files for {' and '.join(p1)}..." + NORMAL)

            print(YELLOW + "  Creating directories..." + NORMAL)
            DIRS = [CRYPTO_SDIR, DRIVERS_SDIR]
            for d in DIRS:
                if not os.path.exists(d):
                    os.makedirs(d)

            print(YELLOW + "  Copying files..." + NORMAL)
            FILES = [
                     (os.path.join(curdir, OUTPUT_ROOT, 'common_crypto', 'crypto_common.h'), CRYPTO_DIR),
                    ]

            # Add all subcomponents to copy.
            # p1: common crypto files (.c/.h)
            for p in p1:
                FILES.append((os.path.join(curdir, OUTPUT_ROOT, 'common_crypto', 'crypto_%s.h' % p), CRYPTO_DIR))
                FILES.append((os.path.join(curdir, OUTPUT_ROOT, 'common_crypto', 'src', 'crypto_%s.c' % p), CRYPTO_SDIR))

            # p2: CAM wrapper files (.c/.h)
            for p in p2:
                FILES.append((os.path.join(curdir, OUTPUT_ROOT, options.ipname, 'wrapper', 'crypto_%s%s_wrapper.h' % (p, ipvalue)), DRIVERS_DIR))
                FILES.append((os.path.join(curdir, OUTPUT_ROOT, options.ipname, 'wrapper', 'src', 'crypto_%s%s_wrapper.c' % (p, ipvalue)), DRIVERS_SDIR))

            # Copy the generated files.
            for f in FILES:
                s, d = f
                print("    %s -> %s" % (s.replace(curdir + os.sep, ''), d))
                shutil.copy(s, d)

    except Exception as ex:
        print(RED + f"\nUnable to copy generated files: {str(ex)}" + NORMAL)
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
        # Specify arguments.
        parser = MyParser(description=__doc__)

        parser.add_argument('-r', '--repository', help='use this pre-existing CryptoV4 repository', type=str)
        parser.add_argument('-c', '--clone', help='attempt to clone/update the repository', action="store_true")
        parser.add_argument('-g', '--git', help='fetch the CryptoV4 repository from this Git link', type=str)
        parser.add_argument('-b', '--branch', help='if cloning/updating, branch name for checkout', type=str)
        parser.add_argument('-i', '--ipname', help='CAM IP to use', type=str, required=True)

        options = parser.parse_args()

        # Convert IP to lowercase by convention
        options.ipname = options.ipname.lower()

        if options.repository is not None:
            if options.clone is True or options.git is not None:
                parser.error("Cannot use a pre-existing repository and specify a repository to clone")

            if not os.path.exists(os.path.join(options.repository, '.git')):
                parser.error(f"Unable to locate repository at '{options.repository}'")
        else:
            options.repository = CRYPTO_V4_REPOSITORY_DEFAULT_PATH

        if options.git is not None:
            if options.clone is False:
                parser.error("Cannot specify a repository to clone without also asking to clone it")
        else:
             options.git = CRYPTO_V4_REPOSITORY_DEFAULT_GIT

        if not os.path.exists(options.repository):
            parser.error(f"No repository found at {options.repository}")

        result = True

        curdir = os.getcwd()
        builddir = os.path.join(curdir, BUILD_ROOT)
        if not os.path.exists(builddir):
            os.makedirs(builddir)

        # Clone the repo if required.
        if options.clone is True:
            if options.branch is None:
                parser.error("Must specify a branch name if cloning the repository.")

            result = update_clone(options)
        else:
            result = check_for_ip(options.repository, options.ipname)

        if result is True:
            result = create_directories(options)

        if result is True:
            print(YELLOW + "\nGenerating files with options:" + NORMAL)
            print(f"  crypto_v4 repo: {options.repository}")

            if options.clone is True:
                print(f"          branch: {options.branch}")

            print(f"              ip: {options.ipname}")
            print("")

            result = generate_files(options)

        if result is True:
            result = copy_generated_files(options)

        if result is True:
            print(GREEN + "Generation complete." + NORMAL)
        else:
            print(RED + "Generation failed!" + NORMAL)

    except Exception as ex:
        print(RED + f"Generation failed: {str(ex)}" + NORMAL)
        print(traceback.format_exc())
        sys.exit(1)

    sys.exit(0)
