FILES_${PN} = ${FIRMWARE_NAME}

do_install() {
	for i in exe.img appdata.img ; do
		crypt-xor -f "${FIRMWARE_NAME}/image/$i.enc" -K "${FIRMWARE_NAME}" -force -q -outfile "${FIRMWARE_NAME}/image/$i"
		rm -f "${FIRMWARE_NAME}/image/$i.enc"
	done
	crypt-xor -f "${FIRMWARE_NAME}/run.sh.enc" -K "${FIRMWARE_NAME}" -force -q -outfile ${FIRMWARE_NAME}/run.sh
	rm -f "${FIRMWARE_NAME}/run.sh.enc"
}

