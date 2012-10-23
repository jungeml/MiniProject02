//Header file for matrixLEDi2c



__u16 block[I2C_SMBUS_BLOCK_MAX];


static __u16 smile_bmp[] = {0x3C, 0x42, 0x95, 0xA1, 0xA1, 0x95, 0x42, 0x3C};
static __u16 frown_bmp[]=  {0x3C, 0x42, 0xA5, 0x91, 0x91, 0xA5, 0x42, 0x3C};
static __u16 neutral_bmp[]={0x3C, 0x42, 0x95, 0x91, 0x91, 0x95, 0x42, 0x3C};
static __u16 my_bmp[] =    {0xA5, 0x42, 0xA5, 0xA5, 0xA5, 0xA5, 0x42, 0xA5};

static void help(void) __attribute__ ((noreturn));




int displayImage(__u16 bmp[], int res, int daddress, int file)
{
	int i;
	for(i=0; i<8; i++)
        {
	     block[i] = (bmp[i]&0xfe) >>1 |
             (bmp[i]&0x01) << 7;
        }
        res = i2c_smbus_write_i2c_block_data(file, daddress, 16,
        	(__u8 *)block);
        sleep(1);

}


static void help(void)
{
	fprintf(stderr, "Usage: my2cset (hardwired to bus 3, address 0x70)\n");
	exit(1);
}

static int check_funcs(int file, int size)
{
	unsigned long funcs;

	/* check adapter functionality */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		fprintf(stderr, "Error: Could not get the adapter "
		"functionality matrix: %s\n", strerror(errno));
		return -1;
	}

	switch (size) {
	case I2C_SMBUS_BYTE:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
			return -1;
		}
		break;

	case I2C_SMBUS_BYTE_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE_DATA)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus write byte");
			return -1;
		}
		break;

	case I2C_SMBUS_WORD_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_WORD_DATA)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus write word");
			return -1;
		}
		break;

	case I2C_SMBUS_BLOCK_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus block write");
			return -1;
		}
		break;
	case I2C_SMBUS_I2C_BLOCK_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)) {
			fprintf(stderr, MISSING_FUNC_FMT, "I2C block write");
			return -1;
		}
		break;
	}

	return 0;
}

