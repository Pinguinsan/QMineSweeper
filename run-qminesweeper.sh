#!/bin/bash

QMineSweeper >> /tmp/$(date +%s)-$$-qminesweeper.log

return $?
