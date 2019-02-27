/*  
    Numerical Integration
	
	Doğukan Öksüz
	root@dogukanoksuz.com.tr
	github.com/dogukanoksuz
    

    file name: integrate.c
*/

#include <stdio.h>
#include <stdlib.h>
#define polynomeFunction fx
#define powerFunction pow_d

int checkDigitType (char a) {
    /*  fonksiyonun açıklaması:
        eğer 1 return olursa a karakteri sayıdır.
        eğer 2 return olursa a karakteri harftir
        eğer 3 return olursa a karakteri + || -'dir
        eğer 0 return olursa hiçbirinin içine girmez.
    */
    if (a >= '0' && a <= '9') {
        return 1;
    } else
    if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')) {
        return 2;
    } else
    if ((a == '-') || (a == '+')) {
        return 3;
    } else {
        return 0;
    }
}

/*--------------------------------------------------------------------*/

// parse ettiğimiz stringdeki katsayıları ve üstleri iki
// ayrı array içine aktaralım ve global olsun ki
// her fonksiyonda kullanabilelim.

signed int katsayiArray[10];
signed int usArray[10];

unsigned int xCursor = 0;
unsigned int uCursor = 0;

int stringToPolynome (char *poly) {
    /*  fonksiyonun açıklaması:
        fonksiyonumuz aldığımız polinomu hane hane okuyarak
        önündeki ve arkasındaki veya bir ilerisindeki karakterleri
        test ederek koşullara göre bunları katsayı/üs arrayimize
        eklemektedir.
    */

    int i = 0;

    while (poly[i]) {
        if (poly[i] == 'x') {
            // polinomda x'den sonra ^ gelmesi şarttır.
            // bu sebeple hata mesajı yazdırıp programı sonlandırıyoruz
            if (poly[i+1] != '^') {
                printf("\npolinomunuz valid değil!\n");
                printf("örnek format: 3x^3+2x^2+5x^1+9x^0\n\n");
                exit(EXIT_FAILURE);
            }

            // düzenli olsun diye x'in bir önündeki haneyi katsayı
            // olarak tanımladım
            int katsayi = poly[i-1] - '0';

            // koşullarımızı test ediyor ona göre katsayıArrayimizi
            // düzenletiyoruz.
            if (checkDigitType(poly[i-1]) == 1 && poly[i-2] == '-') {
                katsayiArray[xCursor] = katsayi * (-1);
            } else
            if (checkDigitType(poly[i-1]) == 1 && poly[i-2] == '+') {
                katsayiArray[xCursor] = katsayi;
            } else
            if (poly[i-1] == '-') {
                katsayiArray[xCursor] = -1;
            } else
            if (poly[i-1] == '+') {
                katsayiArray[xCursor] = 1;
            } else
            if (checkDigitType(poly[i-1]) == 1) {
                katsayiArray[xCursor] = katsayi;
            } else
            if (poly[0] == 'x') {
                katsayiArray[0] = 1;
            }

            // her x bulduğunda da polinomun
            // derecesini belirlememizi sağlayacak sayaç 1 artıyor.
            xCursor++;
        }

        if (poly[i] == '^') {
            // üslerimizi aratıyoruz, ^ işaretinin 1 yanındaki
            // sayı değeri üssümüzdür. ama üssümüz sayı mı diye
            // kontrol edelim.
            int us = poly[i+1] - '0';

            if (checkDigitType(poly[i+1]) == 1) {
                usArray[uCursor] = us;
            } else {
                printf("\n0-9 aralığında üs giriniz.\n\n");
                exit(EXIT_FAILURE);
            }

            uCursor++;
        }

        i++;
    }

    return 0;
}

/*-------------------------------------------------------------------*/

// redefined as pow_d
double powerFunction (double input, int exponent) {
    /*  fonksiyon açıklaması:
        fonksiyonumuz hazır kütüphane kullanmak istemediğimden
        üs alma işlevini gerçekleştirmektedir.
        işlevi sayıyı kendisiyle kuvveti kere çarpmaktır.
        kullanım kolaylığı için powerFunction yerine pow_d olarak
        redefine edilmiştir.
    */
    double result = 1;

    for (int i = 0; i < exponent; i++) {
        result *= input;
    }

    return result;
}

/*-------------------------------------------------------------------*/

// redefined as fx
double polynomeFunction (double x) {
    /*  fonksiyon açıklaması:
        fonksiyonumuz katsayı ve üs arraylerimizden elde ettiğimiz
        değerleri gerçek bir matematiksel fonksiyon haline getiriyor.
        mantığı basit, polinomun derecesi kadar
        katsayi*x^us işlemini yapıp birbiriyle toplamaktadır.
        burası haricinde kullanım kolaylığı olması için
        polynomeFunction yerine fx olarak redefine edilmiştir.
    */
    double result = 0;

    for (int i = 0; i < xCursor; i++) {
        double before = katsayiArray[i] * pow_d(x, usArray[i]);
        result += before;
    }

    return result;
}

/*-------------------------------------------------------------------*/

int stringCompare (char *str1, char *str2) {
    /*  fonksiyon açıklaması:
        iki string arrayin karşılaştırmasını yapmaktadır.
        integralin hesabının türünü tespit etmek için
        kullandım. string.h kütüphanesi yasak olduğundan
        oradaki strcmp fonksiyonu ile aynı işlevi görmektedir.

        nasıl çalışır:
        while döngüsü ile arrayin içinde hane hane gezilir.
        eğer harflerin hepsi birbirine eşitse fonksiyon
        int 1 return eder.
    */
    int i = 0;
    while (str1[i] == str2[i] && str1[i] != '\0')
        i++;
    if (str1[i] > str2[i])
        return 0;
    else if (str1[i] < str2[i])
        return 0;
    else
        return 1;
}

/*-------------------------------------------------------------------*/

int method_id = 0;
int select_method (char *method) {
    /*  fonksiyon açıklaması:
        fonksiyonumuz üstte yazdığım stringCompare fonksiyonunu
        kullanarak aldığımız methodu karşılaştırıp switch yapısında
        kullanmamız için bize bir method_id tanımlar.

        method_id  -> 1 == rectangle
                   -> 2 == midpoint
                   -> 3 == trapezoid
                   -> 4 == simpson
    */

    if(stringCompare(method, "rectangle") == 1
     || stringCompare(method, "Rectangle") == 1) {
        method_id = 1;
    }
    if(stringCompare(method, "midpoint") == 1
     || stringCompare(method, "Midpoint") == 1) {
        method_id = 2;
    }
    if(stringCompare(method, "trapezoid") == 1
     || stringCompare(method, "Trapezoid") == 1) {
        method_id = 3;
    }
    if(stringCompare(method, "simpsons") == 1
     || stringCompare(method, "Simpsons") == 1) {
        method_id = 4;
    }

    return 0;
}

/*-------------------------------------------------------------------*/

/*  fonksiyon açıklamaları:
    alttaki 4 fonksiyonumuz bize gönderilen pdf dosyasındaki
    formülleri matematiksel işlemler olarak gerçekleştirmektedir.
*/

double rectangle (double lowerLimit, double upperLimit) {
    double result;
    double a = lowerLimit;
    double b = upperLimit;

    result = fx(a) * (b - a);

    return result;
}

double midpoint (double lowerLimit, double upperLimit) {
    double result;
    double a = lowerLimit;
    double b = upperLimit;
    double m = (a + b) / 2;

    result = fx(m) * (b - a);

    return result;
}

double trapezoid (double lowerLimit, double upperLimit) {
    double result;
    double a = lowerLimit;
    double b = upperLimit;

    result = (0.5 * fx(a) * (b - a)) + (0.5 * fx(b) * (b - a));

    return result;
}

double simpsons (double lowerLimit, double upperLimit) {
    double result;
    double a = lowerLimit;
    double b = upperLimit;
    double m = (a + b) / 2;

    result = ((b - a) / 6) * (fx(a) + (4 * fx(m)) + fx(b));

    return result;
}

/*-------------------------------------------------------------------*/

int main (int argc, char **argv) {
    // burada 6 argüman var mı yok mu kontrol ediyoruz.
    if (argc != 6) {
        printf("\ngeçerli argüman giriniz\n");
        printf("./integrate polynom lower upper divide method\n");
        printf("tür: rectangle, midpoint, trapezoid, simpsons\n\n");
        exit(EXIT_FAILURE);
    }

    // kullanacağımız değişkenleri tanımlıyoruz.
    char *polynom, *method;
    double lowerLimit, upperLimit, divide;
    polynom      = argv[1];
    lowerLimit   = atof(argv[2]);
    upperLimit   = atof(argv[3]);
    divide       = atof(argv[4]);
    method       = argv[5];

    // fonksiyonumuzu çağırıp stringin parse edilmesini sağlayıp
    // kullanılabilir hale gelmesini sağlıyoruz.
    stringToPolynome(polynom);

    // method_id değişkeninin tanımlanmasını sağlıyoruz.
    select_method(method);

    // kaç parçada hesaplanacak onların işlemlerini ve
    // değişkenlerini tanımlıyoruz.
    double divideAdd = (upperLimit - lowerLimit) / divide;
    double result = 0, temp = 0, before = 0;

    switch (method_id) {
        case 1:
            // rectangle

            // mantık: 3 - 5 arası ve 3 parçaya bölünmesini
            // istiyorsak 5-3 = 2 -> 2/3 = 0,67
            // 3-3,67 - 3,67-4,33 - 4,33-5 şeklinde
            // hesaplamamız gerektiğinden böyle bir
            // for döngüsü yazdım.

            // diğer case'lerde de aynı kod çalışmaktadır.
            for (int i = 0; i < divide; i++) {
                before = rectangle((lowerLimit + temp),
                                (lowerLimit + temp + divideAdd));
                temp   += divideAdd;
                result += before;
            }

            printf("\n%f\n\n", result);
            break;
        case 2:
            // midpoint
            for (int i = 0; i < divide; i++) {
                before = midpoint((lowerLimit + temp),
                                (lowerLimit + temp + divideAdd));
                temp   += divideAdd;
                result += before;
            }

            printf("\n%f\n\n", result);
            break;
        case 3:
            // trapezoid
            for (int i = 0; i < divide; i++) {
                before = trapezoid((lowerLimit + temp),
                                (lowerLimit + temp + divideAdd));
                temp   += divideAdd;
                result += before;
            }

            printf("\n%f\n\n", result);
            break;
        case 4:
            // simpsons
            for (int i = 0; i < divide; i++) {
                before = simpsons((lowerLimit + temp),
                                (lowerLimit + temp + divideAdd));
                temp   += divideAdd;
                result += before;
            }

            printf("\n%f\n\n", result);
            break;

        case 0:
        default:
            printf("\nYanlış argüman tipi.\n");
            printf("rectangle, midpoint, trapezoid, simpsons\n\n");
            exit(EXIT_FAILURE);
            break;
    }
    return 0;
}
