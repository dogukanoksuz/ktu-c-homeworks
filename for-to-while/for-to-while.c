/*
	file: for-to-while.c
	author: Doğukan Öksüz
		root@dogukanoksuz.com.tr
		github.com/dogukanoksuz
*/

#include <stdio.h>
#include <stdlib.h>

int checkFor (char *line) {
	/* fonksiyonun açıklaması:
	   programı sadece 1 forun içindeki 2. foru kontrol edecek şekilde
	   yazabildim. bu sebeple ya ilk hane f sonraki haneleri or (
	   şeklinde tanımlanacaktı ya da ilk hane tab karakteri olacak,
	   diğer hanelerin ise for olup olmadığı kontrol edilecek.

	   eğer nested loop değil ise gördüğü for için 1 değerini döndürür
	   eğer nested loop ise gördüğü for için 2 değerini döndürür.
	*/
	if (line[0] == 'f') {
		if (line[1] == 'o' && line[2] == 'r' && line[4] == '(') {
			return 1;
		}
	} else
	if (line[0] == '\t') {
		if (line[2] == 'o' && line[3] == 'r' && line[5] == '(') {
			return 2;
		}
	}
	return 0;
}

/*--------------------------------------------------------------------*/

char *stringCopy (char *destination, char *source) {
	/* fonksiyon açıklaması:
	   string.h'daki strcpy fonksiyonu ile 1-1 çalışır

	   null gelene kadar loopu döndürür ve haneleri birbirine
	   eşitler.
	*/
	char *start = destination;

	while (*source != '\0') {
		*destination = *source;
		destination++;
		source++;
	}

	*destination = '\0';
	return start;
}

/*--------------------------------------------------------------------*/

int stringLen (char *str) {
	/* fonksiyon açıklaması:
	   string.h'daki strlen fonksiyonunun yaptığı işlemi yapar.

	   null gelene kadar counter arttırır,
	   bu counterin değerini return eder.
	*/
	int i = 0;
	for (i = 0; str[i] != '\0'; ++i);
	return i;
}

/*--------------------------------------------------------------------*/

int main (int argc, char **argv) {
	// file pointerimizi belirliyor ve dosyamızı açıyoruz.
	FILE *inputFile, *outputFile;

	inputFile  = fopen(argv[1], "r");
	outputFile = fopen(argv[2], "w+");

	// eğer dosya açılamadıysa programdan çıkış yap.
	if (inputFile == NULL) exit(EXIT_FAILURE);

	// kullanacağımız arrayleri ve charları tanımlıyoruz.
	int lineCounter = 0;
	char temp[999];
	char lineArr[999][999];

	// burada 2 dimensional array kullandım,
	// parse ettiğim for döngülerini hangi for olduğuna göre
	// ve hangi koşul olduğuna göre arraye atılıp daha sonradan
	// diğer dosyada while döngüsüne değiştirmek için kullanılıyor.
	char variableArr[32][2][100];  // 32  = kaç adet for
								   // 2   = koşullar
								   // 100 = uzunluk

	char nestedVarArr[32][2][100]; // 32  = kaç adet for
								   // 2   = koşullar
								   // 100 = uzunluk

	// input dosyasını okuyoruz ve bütün satırları arrayimize
	// tanımlıyoruz, input dosyası ile işimiz bitiyor.
	while (fgets(temp, sizeof temp, inputFile)) {
		stringCopy(lineArr[lineCounter], temp);
		lineCounter++;
	}

	fclose(inputFile);

	// charlarımızı ve counterlerimizi tanımlıyoruz.
	char *conditions;
	char *nestedConditions;

	int forCounter = 0;
	int nestedForCounter = 0;
	int bracketCounter = 0;
	int nestedBracketCounter = 0;

	for (int i = 0; i < lineCounter; i++) {
		if (checkFor(lineArr[i]) == 1) {
			/*
			   yaptığım parsing methodu:
			   ilk önce for döngüsü geldi mi gelmedi mi test
			   ediyoruz. testimize göre for geldiyse
			   o satırı alıp ilk önce
			   for (kosullar; kosullar; kosullar) { şeklindeki
			   satırımızı kosullar; kosulllar; kosullar) { haline
			   getiriyoruz ki kolay parse edelim.
			*/

			int forLen = 0;
			while (lineArr[i][forLen]) {
				if (lineArr[i][forLen] == '(') {
					conditions = &lineArr[i][forLen + 1];
				}
				forLen++;
			}

			/* ardından aldığım conditions değişkenini parse
			   etmeye başlıyorum. ";" işareti gelene dek bir
			   counter tutturup başlangıç ve bitiş noktam hakkında
			   bilgi sahibi olduktan sonra o counterin aldığı değere
			   kadar olan charları variableArray'ime gönderiyorum.
			   aynı işlemi alttaki nested looplarımda/diğer kısımlarda
			   parse ederken de kullandım.
			*/

			int startLen = 0;
			while (conditions[startLen] != ';') {
				startLen++;
			}
			for (int i = 0; i < startLen; i++) {
				variableArr[forCounter][0][i] = conditions[i];
			}

			/* burada testLen'i startLen+2 olarak tanımlarken o +2'yi
			   koyma sebebim kendi yazdığım formatta koşullar arasında
			   count ederken ; ve bir karakter boşluk olduğundan
			   döngünün düzgün ve estetik parse etmesi için ekledim.
			*/

			int testLen = startLen + 2;
			while (conditions[testLen] != ';') {
				testLen++;
			}
			for (int i = 0; i < testLen - startLen - 2; i++) {
				variableArr[forCounter][1][i] =
							conditions[startLen + 2 + i];
			}

			int countLen = testLen + 2;
			while (conditions[countLen] != ')') {
				countLen++;
			}
			for (int i = 0; i < countLen - testLen - 2; i++) {
				variableArr[forCounter][2][i] =
							conditions[testLen + 2 + i];
			}

			// sonuncu olarak ise belirlediğim şekilde
			// parse ettiğim değerleri while olarak
			// dosyaya bu şekilde yazdırıyorum.
			fprintf(outputFile, "%s;\nwhile (%s) {\n",
				variableArr[forCounter][0],
				variableArr[forCounter][1]);

			forCounter++;
		} else
		if (checkFor(lineArr[i]) == 2) {
			// bu kısımda da aynı işlemler yapılıyor.
			// açıklaması üstte mevcut.

			int forLen = 0;
			while (lineArr[i][forLen]) {
				if (lineArr[i][forLen] == '(') {
					nestedConditions = &lineArr[i][forLen + 1];
				}
				forLen++;
			}

			int startLen = 0;
			while (nestedConditions[startLen] != ';') {
				startLen++;
			}
			for (int i = 0; i < startLen; i++) {
				nestedVarArr[nestedForCounter][0][i] =
								nestedConditions[i];
			}

			int testLen = startLen + 2;
			while (nestedConditions[testLen] != ';') {
				testLen++;
			}
			for (int i = 0; i < testLen - startLen - 2; i++) {
				nestedVarArr[nestedForCounter][1][i] =
								nestedConditions[startLen + 2 + i];
			}

			int countLen = testLen + 2;
			while (nestedConditions[countLen] != ')') {
				countLen++;
			}
			for (int i = 0; i < countLen - testLen - 2; i++) {
				nestedVarArr[nestedForCounter][2][i] =
							 	nestedConditions[testLen + 2 + i];
			}

			fprintf(outputFile, "\t%s;\n\twhile (%s) {\n",
				nestedVarArr[nestedForCounter][0],
				nestedVarArr[nestedForCounter][1]);

			nestedForCounter++;
		} else
		if (lineArr[i][0] == '}') {
			/* bu koşulları yazma sebebim ise } parantezinden
			   önce yazdırmam gereken bir counter olduğundan
			   bu koşulları yazdım.

			   eğer o bracketi gördüğü kısımda counterim eleman
			   tespit ederse düzgün şekilde yazdırıyor.
			   eğer elemanın varlığını tespit edemezse olması
			   gerektiği gibi } işaretini yazdırıp geçiyor.
			   aynı koşul alttakiler için de geçerlidir.
			*/
			if (variableArr[bracketCounter][2][0]) {
				fprintf(outputFile, "\t%s;\n}\n",
					variableArr[bracketCounter][2]);
				bracketCounter++;
			} else {
				fprintf(outputFile, "}\n");
			}
		} else
		if (lineArr[i][1] == '}') {
			if (nestedVarArr[nestedBracketCounter][2][0]) {
				fprintf(outputFile, "\t\t%s;\n\t}\n",
					nestedVarArr[nestedBracketCounter][2]);
				nestedBracketCounter++;
			} else {
				fprintf(outputFile, "\t}\n");
			}
		} else {
			// hiçbir koşula uymuyorsa olduğu gibi satırı yazar.
			fprintf(outputFile, "%s", lineArr[i]);
		}
	}

	fclose(outputFile);

	return 0;
	exit(EXIT_SUCCESS);
}
