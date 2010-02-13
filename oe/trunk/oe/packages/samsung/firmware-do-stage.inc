
do_stage () {
	mkdir -p ${STAGING_DIR}/share/firmware/${FIRMWARE_NAME}
	cp ${FIRMWARE_NAME}/image/exe.img ${STAGING_DIR}/share/firmware/${FIRMWARE_NAME}/
	cp ${FIRMWARE_NAME}/image/appdata.img ${STAGING_DIR}/share/firmware/${FIRMWARE_NAME}/
}

