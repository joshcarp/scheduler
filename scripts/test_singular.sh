#!/bin/bash

i=$1
CASES_DIR="tests/cases"
SCENARIOS_DIR="tests/scenarios"
echo "Test #${i} -- running $(cat ${CASES_DIR}/testcase${i}.in)"
$(cat ${CASES_DIR}/testcase${i}.in) 2>/tmp/stderr >/tmp/stdout
EXIT_CODE=$?
if [ $EXIT_CODE -ne 0 ]; then
    echo "Test #${i} WARNING: did not exit normally -- exit code: $EXIT_CODE"
    echo "========== STDOUT ============"
    cat /tmp/stdout;
    echo "=============================="
    echo "---------- STDERR ------------"
    cat /tmp/stderr;
    echo "------------------------------"
fi

diff ${CASES_DIR}/testcase${i}.out /tmp/stdout &> /tmp/test-output
if [ $? -ne 0 ]; then
    echo -e "Test #${i} FAILED"
    echo "========== DIFF OUTPUT ============"
    cat /tmp/test-output
    echo "==================================="
    ((FAIL++))
    echo "========== STDOUT ============"
    cat /tmp/stdout
    echo "=============================="
    echo "---------- STDERR ------------"
    cat /tmp/stderr
    echo -e "------------------------------\n"
    echo "========== EXPECTED OUTPUT ============"
    cat ${CASES_DIR}/testcase${i}.out
    echo "==================================="
    echo "========== ACTUAL OUTPUT ============"
    cat /tmp/stdout
    echo "==================================="
    rm /tmp/stderr
    rm /tmp/stdout
    exit 1
else
    echo -e "Test #${i} PASSED\n"
fi
((i++))
rm -f /tmp/test-output /tmp/stdout /tmp/stderr