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

LIB_ROOT = os.path.join('..', 'lib')

# Set to show the commands being executed.
SHOW_COMMANDS = True

# Run commands as a set.
def run_commands(cmds, capture_output=False, cwd=None):
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

            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, cwd=cwd)

            # Pull output until done.
            while True:
                output = proc.stdout.readline()
                if proc.poll() is not None:
                    break

                # Print and log the output.
                if output:
                    decoded_line = output.strip().decode("utf-8")

                    # Workaround: FMPP JAR returns code 0 even on failure, so check output for failure instead.
                    # Match lines like "1 failed" but not "0 failed".
                    if "failed" in decoded_line.lower() and "0 failed" not in decoded_line.lower():
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

        print(YELLOW + f"\nUpdating repository at '{repodir}'..." + NORMAL)

        if options.branch is not None:
            GIT_CMDS = ['git fetch',
                        'git checkout %s' % options.branch,
                        'git pull']

            run_commands(GIT_CMDS, cwd=repodir)
        if options.tag is not None:
            GIT_CMDS = ['git fetch',
                    'git checkout %s' % options.tag]

            run_commands(GIT_CMDS, cwd=repodir)

    except Exception as ex:
        print(RED + f"\nFailed to clone repository: {str(ex)}" + NORMAL)
        result = False

    return result


# Copy the input FTL files from CryptoV4 repo
def copy_input_files(repository: str, ip_name: str) -> bool:
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    try:
        ipsrcdir = os.path.join(curdir, repository, 'module_crypto', 'src', 'drivers', ip_name)
        ipdstdir = os.path.join(curdir, INPUT_ROOT, ip_name)

        ccsrcdir = os.path.join(curdir, repository, 'module_crypto', 'src', 'common_crypto')
        ccdstdir = os.path.join(curdir, COMMON_CRYPTO_ROOT)

        templatesrcdir = os.path.join(curdir, repository, 'module_crypto', 'templates')
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

# Parse the sources list and data block from an FMPP config file.
def parse_fmpp_config(config_path: str):
    with open(config_path, 'r') as f:
        content = f.read()

    # Extract sources list.
    sources = []
    sources_match = content.find('sources:')
    if sources_match >= 0:
        bracket_start = content.index('[', sources_match)
        bracket_end = content.index(']', bracket_start)
        sources_block = content[bracket_start + 1:bracket_end]
        for line in sources_block.splitlines():
            line = line.strip().rstrip(',').strip()
            if line and not line.startswith('#'):
                sources.append(line)

    # Extract everything outside the sources block to preserve as the base config.
    # Remove the sources: [...] section and the removeExtensions line.
    lines = content.splitlines()
    data_lines = []
    in_sources = False
    for line in lines:
        stripped = line.strip()
        if stripped.startswith('sources:'):
            in_sources = True
            continue
        if in_sources:
            if ']' in line:
                in_sources = False
            continue
        if stripped.startswith('removeExtensions:'):
            continue
        if stripped.startswith('#'):
            continue
        data_lines.append(line)

    data_block = '\n'.join(data_lines).strip()
    return sources, data_block


# Generate the output files.
def generate_files(ip_name: str, root_folder: str, fmpp_config_name: str) -> bool:
    result = True

    # Path is relative to the script's location.
    curdir = os.path.dirname(os.path.abspath(__file__))

    config_path = os.path.join(curdir, FMPP_ROOT, ip_name, root_folder, f"{fmpp_config_name}.fmpp")
    src_root = os.path.join(curdir, INPUT_ROOT)
    out_root = os.path.join(curdir, OUTPUT_ROOT)

    try:
        print(YELLOW + "\nGenerating files..." + NORMAL)

        # Parse the config and run FMPP once per source file so that a single
        # template error does not block generation of the remaining files.
        sources, data_block = parse_fmpp_config(config_path)
        if not sources:
            print(RED + f"\n  Could not parse sources from '{config_path}'" + NORMAL)
            return False

        failed_sources = []
        temp_config = os.path.join(curdir, BUILD_ROOT, '_temp_single.fmpp')

        for src_file in sources:
            # Write a temporary config with a single source.
            with open(temp_config, 'w') as f:
                f.write("removeExtensions: ftl\n\n")
                f.write(f"sources: [\n    {src_file}\n]\n\n")
                f.write(data_block + "\n")

            cmd = ['fmpp', '-C', temp_config, '-S', src_root, '-O', out_root]
            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
            output_lines = []
            while True:
                line = proc.stdout.readline()
                if proc.poll() is not None:
                    break
                if line:
                    output_lines.append(line.strip().decode("utf-8"))

            failed = any("failed" in l.lower() and "0 failed" not in l.lower() for l in output_lines)
            failed = failed or (proc.poll() != 0)

            if not failed:
                print(CYAN + f"    [ok] {src_file}" + NORMAL)
            else:
                failed_sources.append(src_file)
                print(RED + f"    [FAILED] {src_file}" + NORMAL)
                # Print FMPP error output, skipping status/summary lines.
                for line in output_lines:
                    stripped = line.strip()
                    if not stripped:
                        continue
                    if stripped.startswith("- Executing:") or stripped.startswith("***") or "executed +" in stripped:
                        continue
                    print(RED + f"             {stripped}" + NORMAL)

        # Clean up temporary config.
        if os.path.exists(temp_config):
            os.remove(temp_config)

        if failed_sources:
            print(YELLOW + f"\n  Warning: {len(failed_sources)} template(s) failed:" + NORMAL)
            for s in failed_sources:
                print(YELLOW + f"    - {s}" + NORMAL)

    except Exception as ex:
        print(RED + f"\nFailed to generate files: {str(ex)}" + NORMAL)
        result = False

    return result

# Copy the generated files to their target.
def copy_generated_files(ip_name: str, common_crypto_files: List[str], cam_files: List[str]) -> bool:
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

        crypto_root = os.path.join(curdir, LIB_ROOT, ip_name, 'crypto')
        cc_dir = os.path.join(crypto_root, 'common_crypto')
        cc_sdir = os.path.join(crypto_root, 'common_crypto', 'src')
        drv_dir = os.path.join(crypto_root, 'drivers', 'wrapper')
        drv_sdir = os.path.join(crypto_root, 'drivers', 'wrapper', 'src')

        # Create target directories if they do not exist.
        for d in [cc_dir, cc_sdir, drv_dir, drv_sdir]:
            if not os.path.exists(d):
                os.makedirs(d)

        print(YELLOW + f"\nCopying generated files for {' and '.join(common_crypto_files)}..." + NORMAL)

        FILES = [
                    (os.path.join(curdir, OUTPUT_ROOT, 'common_crypto', 'crypto_common.h'), cc_dir),
                ]

        # Add all subcomponents to copy.
        # common crypto files (.c/.h)
        for p in common_crypto_files:
            FILES.append((os.path.join(curdir, OUTPUT_ROOT, 'common_crypto', 'crypto_%s.h' % p), cc_dir))
            FILES.append((os.path.join(curdir, OUTPUT_ROOT, 'common_crypto', 'src', 'crypto_%s.c' % p), cc_sdir))

        # CAM wrapper files (.c/.h)
        for p in cam_files:
            FILES.append((os.path.join(curdir, OUTPUT_ROOT, ip_name, 'wrapper', 'crypto_%s%s_wrapper.h' % (p, ipvalue)), drv_dir))
            FILES.append((os.path.join(curdir, OUTPUT_ROOT, ip_name, 'wrapper', 'src', 'crypto_%s%s_wrapper.c' % (p, ipvalue)), drv_sdir))

        # Copy the generated files, skipping any that were not produced.
        print(YELLOW + "  Copying files..." + NORMAL)
        skipped = 0
        for f in FILES:
            s, d = f
            if not os.path.exists(s):
                print(YELLOW + "    [skipped] %s (not generated)" % s.replace(curdir + os.sep, '') + NORMAL)
                skipped += 1
                continue
            print(CYAN + "    %s -> %s" % (s.replace(curdir + os.sep, ''), d.replace(curdir + os.sep, '')))
            shutil.copy(s, d)

        if skipped > 0:
            print(YELLOW + f"\n  Warning: {skipped} file(s) were not generated and could not be copied." + NORMAL)

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
        parser.add_argument('-t', '--tag', help='if cloning/updating, tag or commit hash value for checkout', type=str)
        parser.add_argument('-j', '--json', help='path to the generation configuration JSON file', type=str, required=True)

        options = parser.parse_args()

        if options.repository is not None:
            if options.clone is True or options.git is not None:
                parser.error("Cannot use a pre-existing repository and specify a repository to clone")

            if not os.path.exists(options.repository):
                parser.error(f"No repository found at '{options.repository}'")

            if not os.path.exists(os.path.join(options.repository, '.git')):
                parser.error(f"Directory '{options.repository}' is not a Git repository")
        else:
            options.repository = CRYPTO_V4_REPOSITORY_DEFAULT_PATH

        if options.git is not None:
            if options.clone is False:
                parser.error("Cannot specify a repository to clone without also asking to clone it")
        else:
             options.git = CRYPTO_V4_REPOSITORY_DEFAULT_GIT

        any_failed = False
        any_skipped = False
        curdir = os.getcwd()

        # Read in configuration JSON file
        json_path = os.path.join(curdir, options.json)
        if os.path.exists(json_path):
            with open(json_path, 'r') as file:
                generation_config = json.load(file)
        else:
            raise RuntimeError(f"JSON file not found at '{json_path}'")

        # Create the build directory if it doesn't exist
        builddir = os.path.join(curdir, BUILD_ROOT)
        if not os.path.exists(builddir):
            os.makedirs(builddir)

        # Clone the repo if required
        if options.clone is True:
            if options.branch is None and options.tag is None:
                parser.error("Must specify a branch or tag name if cloning the repository.")
            elif options.branch is not None and options.tag is not None:
                parser.error("Must specify a branch or tag name not both.")

            if not update_clone(options):
                raise RuntimeError("Failed to clone/update the repository.")

        for device_name in generation_config["devices"].keys():
            devices_object = generation_config["devices"][device_name]

            ip_name: str = devices_object["ip"].lower()

            if not check_for_ip(options.repository, ip_name):
                any_skipped = True
                print(YELLOW + f"Skipping {device_name} generation as cannot find {ip_name} IP in CryptoV4 repository." + NORMAL)
                continue

            # Ensure input/output directories are empty
            if os.path.exists(INPUT_ROOT):
                shutil.rmtree(INPUT_ROOT)
            if os.path.exists(OUTPUT_ROOT):
                shutil.rmtree(OUTPUT_ROOT)

            if not copy_input_files(options.repository, ip_name):
                print(RED + f"Failed to copy input FTL files for {device_name}!" + NORMAL)
                any_failed = True
                continue

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

                if not generate_files(ip_name, root_folder, fmpp_config):
                    print(RED + f"Failed to generate files for '{project}'!" + NORMAL)
                    any_failed = True
                    continue

                if not copy_generated_files(ip_name, common_crypto_files, cam_files):
                    print(RED + f"Failed to copy generated files for '{project}'!" + NORMAL)
                    any_failed = True
                    continue

                # Sleep for a moment and allow the OS indexing to catch up
                time.sleep(0.1)

        if not any_failed and not any_skipped:
            print(GREEN + "Generation complete." + NORMAL)
        elif any_failed:
            print(RED + "Generation complete with errors. Review log." + NORMAL)
            sys.exit(1)
        else:
            print(BLUE + "Generation complete, but some devices were skipped. Review log." + NORMAL)

    except Exception as ex:
        print(RED + f"Generation failed: {str(ex)}" + NORMAL)
        print(traceback.format_exc())
        sys.exit(1)

    sys.exit(0)
