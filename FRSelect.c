/* ========================================================================= *
* FRSelect
* Implementation of the Floyd-Rivest algorithm
* ========================================================================= */

#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include "Select.h"

static int max(int x, int y){
    if(x > y)
        return x;
    else
        return y;
}

static int min(int x, int y){
    if(x < y)
        return x;
    else
        return y;
}

static int sign(int x){
    if(x < 0)
        return -1;
    else
        return 1;
}

static size_t fr_select(void *array, size_t left, size_t right, size_t k, int (*compare)(const void *, size_t i, size_t j), void (*swap)(void *array, size_t i, size_t j)){
    fprintf(stderr, "1111111111\n");
    while(right > left){
        if(right - left > 600){
            fprintf(stderr, "[INFO] left = %zu, right = %zu, k = %zu\n", left, right, k);
        
            size_t n = right - left + 1;
            size_t i = k - left + 1;
            double z = log(n);
            double s = 0.5 * exp(2 * z / 3);
            double sd = 0.5 * sqrt(z * s * (n - s) / n) * sign(i - n/2);
        
            // 🔥 Affichage des valeurs intermédiaires 🔥
            fprintf(stderr, "[CALCUL] n = %zu, i = %zu, z = %f, s = %f, sd = %f\n", n, i, z, s, sd);
        
            size_t new_left = max(left, (size_t) (k - (i * s / n) + sd));
            size_t new_right = min(right, (size_t) (k + ((n - i) * s / n) + sd));
        
            // 🔥 Vérification des nouvelles bornes 🔥
            fprintf(stderr, "[NOUVELLES BORNES] new_left = %zu, new_right = %zu\n", new_left, new_right);
        
            // 📌 Détection d'erreur potentielle avant la récursion 📌
            if (new_left >= new_right) 
                fprintf(stderr, "⚠️ ERREUR FATALE: new_left (%zu) >= new_right (%zu) → STOP RECURSION\n", new_left, new_right);
            
        
            fprintf(stderr, "✅ Appel récursif: left = %zu, right = %zu, k = %zu, new_left = %zu, new_right = %zu\n", 
                left, right, k, new_left, new_right);
            fr_select(array, new_left, new_right, k, compare, swap);
        }
        fprintf(stderr, "22222222222\n");
        size_t t = k; //Save t position
        size_t i = left;
        size_t j = right;

        swap(array, left, k);
        t = left; //t is now at left

        if(compare(array, right, t) > 0){
            swap(array, left, right);
            t = right; //t is now at right in this case
        }

        while(i < j){
            swap(array, i, j);

            if(t == i)
                t = j;
            else if(t == j)
                t = i;

            i++;
            j--;

            while(i < right && compare(array, i, t) < 0)
                i++;
            while(j > left && compare(array, j, t) > 0)
                j--;  
        }
        fprintf(stderr, "33333333333333\n");

        if(compare(array, left, t) == 0){
            swap(array, left, j);
            if(t == left)
                t = j;
            else if(t == j)
                t = left;
        }
        else{
            j++;
            if(j <= right){
                swap(array, j, right);
                if(t == j)
                    t = right;
                else if(t == right)
                    t = j;
            }
        }

        if(j <= k)
            left = j + 1;
        if(k <= j)
            right = j - 1;
    }

    return k;
}

size_t select(void *array, size_t length, size_t k,
    int (*compare)(const void *, size_t i, size_t j),
    void (*swap)(void *array, size_t i, size_t j))
{

return fr_select(array, 0, length - 1, k, compare, swap);
}

