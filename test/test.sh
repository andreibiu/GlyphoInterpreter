#!/bin/bash

TEST_SUITES=${@:-test bigtest extra bigextra error exception exceptionextra anybase biganybase exceptionanybase}
TEST_DIR=`dirname $0`
IN_DIR="$TEST_DIR/in"
OUT_DIR="$TEST_DIR/out"
REF_DIR="$TEST_DIR/ref"
LOG_DIR="$TEST_DIR/log"

run_tests () {
    for TEST_SUITE in ${TEST_SUITES}; do
        for SRC_FILE in `find ${IN_DIR} -iname "${TEST_SUITE}[0-9]*.gly"`; do
            TEST_NAME=`basename ${SRC_FILE/.gly/}`
            IN_FILE="$IN_DIR/$TEST_NAME.in"
            OUT_REF_FILE="$OUT_DIR/$TEST_NAME.out"
            ERR_REF_FILE="$OUT_DIR/$TEST_NAME.err"
            RETVAL_REF=`cat "$OUT_DIR/$TEST_NAME.ret"`

            RAW_OUT_FILE="$OUT_DIR/$TEST_NAME.rawout"
            OUT_FILE="$OUT_DIR/$TEST_NAME.out"
            RAW_ERR_FILE="$OUT_DIR/$TEST_NAME.rawerr"
            ERR_FILE="$OUT_DIR/$TEST_NAME.err"
            RET_FILE="$OUT_DIR/$TEST_NAME.ret"

            LOG_COM_FILE="$LOG_DIR/$TEST_NAME.command"

            rm ${RAW_OUT_FILE} ${RAW_ERR_FILE} ${OUT_FILE} ${ERR_FILE} ${LOG_COM_FILE} &> /dev/null

            unset BASE
            if [[ $TEST_NAME =~ .*anybase.* ]]; then
                BASE=`grep ^${TEST_NAME} ${IN_DIR}/base.cfg | cut -d ' ' -f 2`
            fi

            make -s run source=${SRC_FILE} base=${BASE} < ${IN_FILE} > ${RAW_OUT_FILE} 2> ${RAW_ERR_FILE}

            cat ${RAW_OUT_FILE} | grep -vi Makefile > "$OUT_FILE"
            diff -bBq "${OUT_FILE}" "${OUT_REF_FILE}" &> /dev/null
            OUT_CMP=$?

            MAKE_ERR_LINE=`cat ${RAW_ERR_FILE} | sed 's/make.* /\nmake: /' | grep ^make`
            RETVAL="${MAKE_ERR_LINE##* }"
            RETVAL="${RETVAL:-0}"

            cat ${RAW_ERR_FILE} | sed 's/make.* /\nmake: /' | grep -v ^make > ${ERR_FILE}
            diff -bBq "${ERR_FILE}" "${ERR_REF_FILE}" &> /dev/null
            ERR_CMP=$?

            if [ "$OUT_CMP" = "0" ] && [ "$ERR_CMP" = "0" ] && [ "$RETVAL" = "$RETVAL_REF" ]; then
                echo -e "\e[32mPASSED\e[0m Test \e[1;33m${TEST_NAME}\e[0m"
                rm ${RAW_OUT_FILE} ${OUT_FILE} ${ERR_FILE} ${RAW_ERR_FILE} 2>/dev/null
            else
                echo -e "\e[31mFAILED\e[0m Test $\e[1;33m${TEST_NAME}\e[0m"
                echo ${RETVAL} > ${RET_FILE}
                echo "make -s run input=${SRC_FILE} base=$BASE < ${IN_FILE} > ${RAW_OUT_FILE} 2> ${RAW_ERR_FILE}" > ${LOG_COM_FILE}
                echo "Output comparison: ${OUT_CMP}, expected 0"
                echo "Error comparison: ${ERR_CMP}, expected 0"
                echo "Return value comparison: ${RETVAL}, expected ${RETVAL_REF}"
            fi
        done
    done
}


echo "Building..."
make build
mkdir -p ${OUT_DIR}
mkdir -p ${LOG_DIR}
rm -f ${LOG_DIR}/*

echo "Runing tests..."
run_tests

echo "Finishing..."
make clean
