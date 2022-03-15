

unsigned char reflect_byte(unsigned char *byte, unsigned char *byte_ref, unsigned char index){
    unsigned char i, j, temp;
    for (j = 0; j < index; j++) {
        for (i = 0; i < 8; i++) {
            temp = byte[j] >> i;
            byte_ref[index - 1 - j] += (temp & 0x01) << (7 - i);
           
        }
    }
   //return *byte_ref;
}



