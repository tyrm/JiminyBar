#!/bin/bash
# Create a user called jiminy for the agent to run as
su -c "nohup ./jiminy.js >/dev/null &" -s /bin/sh jiminy
