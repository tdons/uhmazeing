#!/bin/sh

for test in build/test-*.test
do
  printf .
  output=$(./"$test" 2>&1)
  exit_code=$?
  if [ $exit_code -ne 0 ]; then
    >&2 printf '\n%s failed with exit code %s. Output:\n%s' "$(basename "${test}")" "$exit_code" "$output"
    exit $?
  fi
done
echo " OK"
