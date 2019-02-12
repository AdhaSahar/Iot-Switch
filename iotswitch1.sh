#!/bin/sh

mosquitto_pub -h localhost -t "switch1/status" -m "$status"
