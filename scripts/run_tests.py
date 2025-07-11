# This file is a part of the SIRF (Simple Intermediate Representation Format)
# project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

import subprocess
import os
from loguru import logger

TESTS_DIR="./tests/auto"
BINARY="./build/sirfc"

def run_test(test_file):
    # Construct the command to run your interpreter with the test file
    cresult = subprocess.run([BINARY, "-o", test_file + ".out", test_file, "-O", "0", "-e", "exe"])
    if cresult.returncode != 0:
        return False
    
    result = subprocess.run([test_file + ".out"], capture_output=True, text=True)

    # Get the output of the program
    actual_output = result.stdout.strip()

    # Construct the expected output file path
    expected_output_file = f"{test_file}.out"

    # Read the expected output
    with open(expected_output_file, "r") as f:
        expected_output = f.read().strip()

    # Compare actual output with expected output
    if actual_output == expected_output:
        logger.info(f"Test passed: {test_file}")
    else:
        logger.error(f"Test failed: {test_file}")
        logger.info(f"Expected:\n{expected_output}")
        logger.info(f"File stdout:\n{actual_output}")
        logger.info(f"File stderr:\n{result.stderr.strip()}")
        return False
    return True

def run_all_tests():
    all_tests_passed = True

    # Run all the test files in the tests directory
    for test_file in os.listdir(TESTS_DIR):
        if test_file.endswith(".sirf"):
            test_file_path = os.path.join(TESTS_DIR, test_file)
            if not run_test(test_file_path):
                all_tests_passed = False

    return all_tests_passed

if __name__ == "__main__":
    all_tests_passed = run_all_tests()
    if all_tests_passed:
        logger.info("All tests passed!")
        exit(0)
    else:
        logger.error("Some tests failed.")
        exit(1)