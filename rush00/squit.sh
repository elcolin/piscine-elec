#!/bin/sh

# Little script to quit active screen session

# Get the screen information
screen_info=$(screen -ls)

# Extract the field using awk
screen_name=$(echo "$screen_info" | awk 'NR==2 {print $1}')

# Output the extracted field
echo "Quitting: $screen_name"

# Quit the screen session
screen -XS $screen_name quit

