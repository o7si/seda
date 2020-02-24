#!/bin/sh

ragel -G2 -C httpclient_parser.rl -o httpclient_parser.cpp
ragel -G2 -C http11_parser.rl -o http11_parser.cpp
