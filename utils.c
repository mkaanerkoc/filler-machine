/*
 * utils.c
 *
 *  Created on: 26 Ara 2015
 *      Author: 21128195, Mehmet Kaan ERKOÇ
 *      This library file consist of basic util functions for our project.Converting string into float , calculating turn count v.s. v.s. 
 */


void delaySec(unsigned int ms) {
	while (ms--) {
		__delay_cycles(10000); // set for 16Mhz change it to 1000 for 1 Mhz
	}
}


void getWeightInfo(char a[], char b[]) {
	char i = 0;
	for (i = 0; i < 5; i++) {
		b[i] = a[i + 4];
	}
}



void getWeightStr(char a[], char b[]) {
	b[0] = '\0';
	if (a[0] == 48) {
		if (a[1] == 48) {
			if (a[2] == 48) {
				b[0] = a[3];
				b[1] = 46;
				b[2] = a[4];
				b[3] = ' ';
				b[4] = ' ';
				b[5] = '\0';
			} else {
				b[0] = a[2];
				b[1] = a[3];
				b[2] = 46;
				b[3] = a[4];
				b[4] = ' ';
				b[5] = '\0';
			}
		} else {
			b[0] = a[1];
			b[1] = a[2];
			b[2] = a[3];
			b[3] = 46;
			b[4] = a[4];
			b[5] = ' ';
		}
	} else {
		b[0] = a[0];
		b[1] = a[1];
		b[2] = a[2];
		b[3] = a[3];
		b[4] = 46;
		b[5] = a[4];
		b[6] = '\0';
	}
}
void clearStr(char a[]) {
	a[0] = '\0';
}
void resetStr(char a[]) {
	char i = 0;
	for(i=0;i<5;i++){
		a[i]='\0';
	}
}
int attenuate(float errval) {
	if (errval > 125)
		return 2400;
	else if (errval <= 125 && errval > 75)
		return 1600;
	else if (errval <= 75 && errval > 30)
		return 800;
	else if (errval <= 30 && errval > 10)
		return 100;
	else if (errval <= 10 && errval > 6)
		return 30;
	else if (errval <= 6 && errval > 4)
		return 15;
	else if (errval <= 4 && errval > 2)
		return 6;
	else if (errval <= 2.0f && errval > 1.0f)
		return 3;
	else if (errval <= 1.0f && errval > 0.3f)
		return 2;
	else if (errval <= 0.3f) {
		return 0;
	}

}
void append(char a[], const char b) {
	char i = 0;
	for (i = 0; i < 32; i++) {
		if (a[i] == '\0') {
			a[i] = b;
			a[i + 1] = '\0';
			break;
		}
	}
}
float stringToFloat(char a[], int tarti) {
	char i = 0;
	float retVal = 0;
	if (tarti) {
		for (i = 0; i < 5; i++) {
			if (i == 0)
				retVal += (a[i] - 48) * 10000;
			else if (i == 1)
				retVal += (a[i] - 48) * 1000;
			else if (i == 2)
				retVal += (a[i] - 48) * 100;
			else if (i == 3)
				retVal += (a[i] - 48) * 10;
			else if (i == 4)
				retVal += (a[i] - 48);
		}
		return retVal / 10;
	} else {
		int length = 0;
		for (i = 0; i < 3; i++) {
			if (i == 0)
				retVal += (a[i] - 48) * 100;
			else if (i == 1)
				retVal += (a[i] - 48) * 10;
			else if (i == 2)
				retVal += (a[i] - 48);
		}
		return retVal;
	}
}
void floatToStr(float arg, char arg1[]) {

	int r = arg * 10;
	int bin = r / 10000;
	int yuz = (r % 10000) / 1000;
	int on = (r % 1000) / 100;
	int bi = (r % 100) / 10;
	int kus = r % 10;
	if (bin == 0 && yuz != 0) {
		arg1[0] = yuz + 48;
		arg1[1] = on + 48;
		arg1[2] = bi + 48;
		arg1[3] = 46;
		arg1[4] = kus + 48;
		arg1[5] = ' ';
	} else if (bin == 0 && yuz == 0 && on != 0) {
		arg1[0] = on + 48;
		arg1[1] = bi + 48;
		arg1[2] = 46;
		arg1[3] = kus + 48;
		arg1[4] = ' ';
		arg1[5] = ' ';
	} else if (bin == 0 && yuz == 0 && on == 0) {
		arg1[0] = bi + 48;
		arg1[1] = 46;
		arg1[2] = kus + 48;
		arg1[3] = ' ';
		arg1[4] = ' ';
		arg1[5] = ' ';
	} else {
		arg1[0] = bin + 48;
		arg1[1] = yuz + 48;
		arg1[2] = on + 48;
		arg1[3] = bi + 48;
		arg1[4] = 46;
		arg1[5] = kus + 48;
		arg1[6] = '\0';
	}
}


