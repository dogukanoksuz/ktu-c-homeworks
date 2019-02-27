/* 
   File: isbn10.c
   Author: Doğukan Öksüz
	   root@dogukanoksuz.com.tr
	   github.com/dogukanoksuz
                                 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*---------------------------------------------------------------*/

// bir int array tanımlıyoruz.
int d[10];
long long split_parameter(long long parameter) {
    /* bu fonksiyonda
       9 haneli gelen sayıyı for döngüsü aracılığı ile
       parçalayıp arrayımıza atıyoruz ki
       işlemlerimizi yapabilelim.                                */

    for (int i = 2; i <= 10; i++) {
        d[i] = parameter % 10;
        parameter /= 10;
    }

    /* hanelere ayırabilmek için 10 ile modunu alıp 10'a böldük.
       parametremiz her 10'a bölündüğünde 1 basamak azalacağından
       bizim arrayimize basamakların eklenmesini sağladı.

       d[1], d[2] olarak tanımlamamın sebebi örnek gönderilen
       dosyada bize bu şekilde verilmiş olması, ben de düzeni
       bozmak istemedim.

       d[1]  özel hanemiz
       d[10] en büyük basamak
       d[2]  en küçük basamak                                    */

    return 0;
}

/*---------------------------------------------------------------*/

long long calculate_special_digit(long long parameter) {
    /* bu fonksiyonda ise parçaladığımız sayıları işleme sokup
       özel haneyi hesaplama işlemlerini yapacağız.

       kurala uygun olarak sayıları çarpıp 11 ile bölümünü bulalım
       11'in modundan geleni 11'e tamamlamamız gerek ki toplamı
       11'e kalansız bölünebilsin.                              */

    // aldığımız parametrenin parçalanmasını sağlıyoruz
    split_parameter(parameter);

    long long before, calculate_function = 0;
    // hesaplama işlemlerini yapıyoruz,
    for (int i = 2; i <= 10; i++) {
        // yaptığım işlem aslında 2*d[2] + 3*d[3]... + n*d[n]
        before = i * d[i];
        calculate_function += before;
    }
    d[1]                = 11 - (calculate_function % 11);

    if (d[1] == 11) {
        // 11 çıkabiliyor 0 çıktığında 11'den çıkardığımız için
        // o koşulu düzeltelim.
        d[1]            = 0;
    }

    /* sayıları toparlıyoruz
       overflow problemi yüzünden long long int tanımlıyoruz.
       10 haneli 2147'den itibaren başlayan sayıda overflowa
       düşüyor, hatalı sonuç elde etmemek için long long kullandık
                                                                */
    long long result;

    if (d[1] <= 9) {
        // değerimizi hesaplatıyoruz.
        result = parameter * 10 + d[1];
        printf("ISBN-10 Calculated Digit: \n%lld\n\n", result);
    } else {
        // 10 gelen koşulda X olarak yazdırıyorum.
        printf("ISBN-10 Calculated Digit: \n%lldX\n\n", parameter);
    }

    return 0;
}

/*---------------------------------------------------------------*/

// terminalden argümanı çekiyoruz.

int main(int argc, char *argv[]) {
    /* main fonksiyonumuzu yazdığımız fonksiyonların
       belirlediğimiz koşulların kontrol edildikten
       sonra çalıştırılması için kullanıyoruz.

       argüman girildi mi kontrol ediyoruz                       */

    if (argc >= 2) {
        // atoi fonksiyonu ile ascii'ı integer yapıyoruz.
        long long parameter = atoi(argv[1]);

        // 9 haneden fazla mı diye bakıyoruz
        if (parameter < 1000000000 && parameter >= 0) {
            // listeli parametremizi hesaplatıyoruz ve yazdırıyoruz
            calculate_special_digit(parameter);
        } else {
            printf("9 haneden buyuk verdin. \n");
            printf("\x1B[32m./assignment1 9-digit\n\x1B[0m");
        }
    } else {
        printf("eksik veya fazla parametre girdin. \n");
        printf("\x1B[32m./assignment1 9-digit\n\x1B[0m");
    }

    return 0;
}
