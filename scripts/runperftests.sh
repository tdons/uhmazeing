#!/usr/bin/env bash

for t in build/perf-*.test
do
  ./"$t"
done
