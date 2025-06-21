make:
	pio run

upload:
	pio run -t upload

serial:
	pio device monitor --baud 115200

uploadserial:
	make upload
	sleep 1.5
	make serial

compiledb:
	pio run -t compiledb

