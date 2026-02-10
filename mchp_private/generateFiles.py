#!/bin/python3

import json
import time
import argparse, os, sys, traceback, subprocess
import shutil
from typing import List

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

CRYPTO_V4_REPOSITORY_NAME = 'crypto_v4'
CRYPTO_V4_REPOSITORY_DEFAULT_PATH = os.path.join(BUILD_ROOT, CRYPTO_V4_REPOSITORY_NAME)
CRYPTO_V4_REPOSITORY_DEFAULT_GIT = 'https://bitbucket.microchip.com/scm/mh3/%s.git' % CRYPTO_V4_REPOSITORY_NAME

# Common target crypto firmware path.
CRYPTO_FIRMWARE_PATH = os.path.join('crypto')

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
            result = True
            
            # If the command is already a list, use it as-is.
            if type(c) is list:
                cmd = c
            else:
                cmd = c.split(' ')

            if SHOW_COMMANDS is True:
                print(CYAN + f"\nRunning command '{' '.join(cmd)}'" + NORMAL)

            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)

            # Pull output until done.
            while True:
                output = proc.stdout.readline()
                if proc.poll() is not None:
                    break

                # Print and log the output.
                if output:
                    decoded_line = output.strip().decode("utf-8")

                    # Workaround: FMPP JAR returns code 0 even on failure, so check output for failure instead
                    if "Failed" in decoded_line:
                        result = False

                    if capture_output:
                        captured_output.append(decoded_line)
                    else:
                        print(decoded_line)

            if result is False:
                raise Exception(f"Command '{' '.join(cmd)}' failed")
            else:
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
def check_for_ip(repodir, ip_name: str):
    result = True

    ipdir = os.path.join(repodir, 'module_crypto', 'src', 'drivers', ip_name)
    if not os.path.exists(ipdir):
        print(RED + f"\nIP '{ip_name}' does not exist in the repository at '{repodir}'." + NORMAL)
        result = False

    return result


# Clone / update the repository.
def update_clone(options) -> bool:
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

    except Exception as ex:
        print(RED + f"\nFailed to clone repository: {str(ex)}" + NORMAL)
        result = False

    os.chdir(curdir)

    return result


# Create all directories.
def create_directories(ip_name: str) -> bool:
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        print(YELLOW + "\nCreating directories..." + NORMAL)

        inputdir = os.path.join(curdir, INPUT_ROOT)
        ipdir = os.path.join(curdir, inputdir, ip_name)
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

# Copy the input FTL files from CryptoV4 repo
def copy_input_files() -> bool:
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        ipsrcdir = os.path.join(curdir, options.repository, 'module_crypto', 'src', 'drivers', ip_name)
        ipdstdir = os.path.join(curdir, INPUT_ROOT, ip_name)

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

                print(CYAN + "  %s -> %s" % (srcdir.replace(curdir + os.sep, ''), dstdir.replace(curdir + os.sep, '')))
                shutil.copytree(srcdir, dstdir)

            except Exception as ex:
                print(RED + f"Failed to copy files from '{srcdir}' to '{dstdir}': {str(ex)}" + NORMAL)
                print(traceback.format_exc())
                result = False
                break
    except Exception as ex:
        print(RED + f"\nUnable to build paths for copying input files: {str(ex)}" + NORMAL)
        result = False

    return result

# Generate the output files.
def generate_files(ip_name: str, root_folder: str, fmpp_config_name: str) -> bool:
    result = True

    try:
        # Run FMPP to generate the files.
        print(YELLOW + "\nGenerating files..." + NORMAL)

        FMPP_CMD = [['fmpp',
                    '-C', os.path.join(curdir, FMPP_ROOT, ip_name, root_folder, f"{fmpp_config_name}.fmpp"),
                    '-S', os.path.join(curdir, INPUT_ROOT),
                    '-O', os.path.join(curdir, OUTPUT_ROOT)]
                ]

        try:
            run_commands(FMPP_CMD)
        except Exception as e:
            print(str(e))
            result = False

    except Exception as ex:
        print(RED + f"\nFailed to generate files: {str(ex)}" + NORMAL)
        result = False

    return result

# Copy the generated files to their target.
def copy_generated_files(ip_name: str, device: str, project_name: str, project_root_dir: str, common_crypto_files: List[str], cam_files: List[str]) -> bool:
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        try:
            _, ipvalue = ip_name.split('_', 1)

        except Exception as ex:
            print(RED + f"Unable to determine IP value from '{ip_name}': {str(ex)}" + NORMAL)
            print(traceback.format_exc())
            return False
        
        apps_root = os.path.join('..', device)
        project_dir_path = os.path.join(apps_root, project_root_dir, f"{project_name}.X", CRYPTO_FIRMWARE_PATH)

        CRYPTO_DIR = os.path.join(project_dir_path, 'common_crypto')
        CRYPTO_SDIR = os.path.join(project_dir_path, 'common_crypto', 'src')

        DRIVERS_DIR = os.path.join(project_dir_path, 'drivers', 'wrapper')
        DRIVERS_SDIR = os.path.join(project_dir_path, 'drivers', 'wrapper', 'src')

        print(YELLOW + f"\nCopying files for '{project_name.upper()}'..." + NORMAL)
        print(YELLOW + f"\n  Processing files for {' and '.join(common_crypto_files)}..." + NORMAL)

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
        # common crypto files (.c/.h)
        for p in common_crypto_files:
            FILES.append((os.path.join(curdir, OUTPUT_ROOT, 'common_crypto', 'crypto_%s.h' % p), CRYPTO_DIR))
            FILES.append((os.path.join(curdir, OUTPUT_ROOT, 'common_crypto', 'src', 'crypto_%s.c' % p), CRYPTO_SDIR))

        # CAM wrapper files (.c/.h)
        for p in cam_files:
            FILES.append((os.path.join(curdir, OUTPUT_ROOT, ip_name, 'wrapper', 'crypto_%s%s_wrapper.h' % (p, ipvalue)), DRIVERS_DIR))
            FILES.append((os.path.join(curdir, OUTPUT_ROOT, ip_name, 'wrapper', 'src', 'crypto_%s%s_wrapper.c' % (p, ipvalue)), DRIVERS_SDIR))

        # Copy the generated files.
        for f in FILES:
            s, d = f
            print(CYAN + "    %s -> %s" % (s.replace(curdir + os.sep, ''), d))
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
        parser.add_argument('-j', '--json', help='path to json configuration file', type=str, required=True)

        options = parser.parse_args()

        if options.repository is not None:
            if options.clone is True or options.git is not None:
                parser.error("Cannot use a pre-existing repository and specify a repository to clone")

            if not os.path.exists(os.path.join(options.repository, '.git')):
                parser.error(f"Unable to locate repository at '{options.repository}'")

            if not os.path.exists(options.repository):
                parser.error(f"No repository found at {options.repository}")
        else:
            options.repository = CRYPTO_V4_REPOSITORY_DEFAULT_PATH

        if options.git is not None:
            if options.clone is False:
                parser.error("Cannot specify a repository to clone without also asking to clone it")
        else:
             options.git = CRYPTO_V4_REPOSITORY_DEFAULT_GIT

        result = True
        skipped = False
        curdir = os.getcwd()

        # Read in configuration JSON file
        if options.json is not None:
            json_path = os.path.join(curdir, options.json)
            if os.path.exists(json_path):
                with open(json_path, 'r') as file:
                    generation_config = json.load(file)
            else:
                raise RuntimeError("JSON file not found at provided path")
        else:
            parser.error(f"Please provide a path to a configuration JSON file")

        # Create the build directory if it doesn't exist
        builddir = os.path.join(curdir, BUILD_ROOT)
        if not os.path.exists(builddir):
            os.makedirs(builddir)

        # Clone the repo if required
        if options.clone is True:
            if options.branch is None:
                parser.error("Must specify a branch name if cloning the repository.")

            result = update_clone(options)
        
        if result is True:

            for device_name in generation_config["devices"].keys():
                devices_object = generation_config["devices"][device_name]

                ip_name: str = devices_object["ip"].lower()
                result = check_for_ip(options.repository, ip_name)

                if result is True:
                    result = create_directories(ip_name)
                else:
                    print(RED + "Failed to create directories!" + NORMAL)

                # if ip exists for device, begin generation and copying to projects
                if result is True:

                    # Ensure input/output directories are empty
                    if os.path.exists(INPUT_ROOT):
                        shutil.rmtree(INPUT_ROOT)
                    if os.path.exists(OUTPUT_ROOT):
                        shutil.rmtree(OUTPUT_ROOT)

                    result = copy_input_files()

                    if result is not True:
                        print(RED + "Failed to copy input FTL files!" + NORMAL)
                    else:
                        for project in devices_object["projects"].keys():
                            project_object = devices_object["projects"][project]
                            root_folder: str = project_object["root_folder"]
                            fmpp_config: str = project_object["fmpp_config"]
                            common_crypto_files: List[str] = project_object["common_crypto_files"]
                            cam_files: List[str] = project_object["cam_files"]

                            print(YELLOW + f"\nGenerating files for {device_name[:2] + device_name[2:].upper()} '{project.upper()}' project with options:" + NORMAL)
                            print(CYAN + f"  crypto_v4 repo: {options.repository}" + NORMAL)

                            if options.clone is True:
                                print(CYAN + f"    branch: {options.branch}" + NORMAL)

                            print(CYAN + f"  ip: {ip_name}" + NORMAL)
                            print(CYAN + f"  fmpp config: {fmpp_config}" + NORMAL)
                            
                            result = generate_files(ip_name, root_folder, fmpp_config)

                            if result is True:
                                result = copy_generated_files(ip_name, device_name, project, root_folder, common_crypto_files, cam_files)

                                if result is not True:
                                    print(RED + "Failed to copy generated files!" + NORMAL)
                            else:
                                print(RED + "Failed to generate files!" + NORMAL)

                            # Sleep for a moment and allow the OS indexing to catch up
                            time.sleep(0.1)
                else:
                    skipped = True
                    print(YELLOW + f"Skipping {device_name} generation as cannot find {ip_name} IP in CryptoV4 repository." + NORMAL)

        if result is True:
            print(GREEN + "Generation complete." + NORMAL)
        elif result is False and skipped is True:
            print(BLUE + "Generation complete, but with errors. Review log." + NORMAL)
        else:
            print(RED + "Generation failed!" + NORMAL)

    except Exception as ex:
        print(RED + f"Generation failed: {str(ex)}" + NORMAL)
        print(traceback.format_exc())
        sys.exit(1)

    sys.exit(0)
