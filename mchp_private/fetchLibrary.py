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

LIBRARIES_ROOT = "libraries"
INCLUDE_ROOT = os.path.join("driver", "include")

LIBRARY_NAME = 'libcam05346-dspic33a.a'

APPS_ROOT = os.path.join('..', 'dspic33ak512mps512')
CRYPTO_LIBRARY_PATH = os.path.join('crypto', 'drivers', 'library')

HASH_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'hash', 'sha.X', CRYPTO_LIBRARY_PATH)
AES_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'aes', 'aes.X', CRYPTO_LIBRARY_PATH)
ECDSA_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'dsa', 'ecdsa.X', CRYPTO_LIBRARY_PATH)
ECDH_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'ecdh', 'ecdh.X', CRYPTO_LIBRARY_PATH)
TRNG_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'trng', 'trng.X', CRYPTO_LIBRARY_PATH)
AES_HASH_CRYPTO_LIBDIR = os.path.join(APPS_ROOT, 'aes_hash', 'aes_hash.X', CRYPTO_LIBRARY_PATH)

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
                print("\nRunning command '%s'" % ' '.join(cmd))

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
                raise Exception("Command '%s' failed with result code %d" % (' '.join(cmd), retcode))

        except OSError:
            raise Exception("Failed to start execution of command '%s'" % ' '.join(cmd))

    if True == capture_output:
        return captured_output
    else:
        return None

# Copy the build files to the target location
def install(options):
    result = True
    curdir = os.getcwd()

    try:
        COPIES = [('hash', HASH_CRYPTO_LIBDIR, ['version', 'hash']),
                  ('aes', AES_CRYPTO_LIBDIR, ['version', 'aes']),
                  ('dsa', ECDSA_CRYPTO_LIBDIR, ['version', 'ecdsa', 'pke']),
                  ('ecdh', ECDH_CRYPTO_LIBDIR, ['version', 'ecdh', 'pke']),
                  ('trng', TRNG_CRYPTO_LIBDIR, ['version', 'trng']),
                  ('aes+hash', AES_HASH_CRYPTO_LIBDIR, ['version', 'hash', 'aes'])]

        print(YELLOW + "\nCopying files from '%s' to '%s'..." % (options.install, APPS_ROOT) + NORMAL)
        for c in COPIES:
            name, d, p1 = c

            srcdir = os.path.join(curdir, options.install, LIBRARIES_ROOT)
            dstdir = os.path.join(curdir, d)

            FILES = [(os.path.join(srcdir, LIBRARY_NAME), os.path.join(dstdir, LIBRARY_NAME))]
            for p in p1:
                headerfile = 'cam_%s.h' % p
                FILES.append((os.path.join(srcdir, headerfile), os.path.join(dstdir, headerfile)))

            print(YELLOW + "\nCopying %s files..." % name + NORMAL)
            for f in FILES:
                s, d = f
                print("  %s -> %s" % (s.replace(curdir + os.sep, ''), d.replace(curdir + os.sep, '')))
                shutil.copy(s, d)

        print(" ")

    except Exception as ex:
        print(RED + "\nFailed to install files: %s" % str(ex) + NORMAL)
        result = False

    return result


# Main
if __name__ == "__main__":

    # Defined this class to support verbose help on argument error.
    class MyParser(argparse.ArgumentParser):
        def error(self, message):
            sys.stderr.write(RED + 'error: %s\n' % message + NORMAL)
            self.print_help()
            sys.exit(2)

    try:
        # Specify arguments.
        parser = MyParser(description=__doc__)

        parser.add_argument('-i', '--install', help='install libraries from library directory', type=str, required=True)

        options = parser.parse_args()

        if options.install is not None and not os.path.exists(options.install):
            parser.error("Library installation directory '%s' not found." % options.install)

        if options.install is not None:
            install(options)

        print(GREEN + "Installation complete." + NORMAL)

    except Exception as ex:
        print(RED + "Installation failed: %s" % str(ex) + NORMAL)
        print(traceback.format_exc())
        sys.exit(1)

    sys.exit(0)
