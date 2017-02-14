//
// Created by xunda on 2/12/2017.
//
#include <jni.h>
#include <string.h>
#include <stdio.h>

#define SIGNAL_LIST_MAX      220500
#define SIGNAL_FILE          "hw1_one_col.dat"
#define M_PI 3.14159265358979323846
#define FILTER_MAX 416
#define ELEMENT_COUNT(X) (sizeof(X) / sizeof((X)[0]))

void convolve(const double Signal[/* SignalLen */], size_t SignalLen,
              const double Kernel[/* KernelLen */], size_t KernelLen,
              double Result[/* SignalLen + KernelLen - 1 */])
{
    size_t n;

    for (n = 0; n < SignalLen + KernelLen - 1; n++)
    {
        size_t kmin, kmax, k;

        Result[n] = 0;

        kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
        kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

        for (k = kmin; k <= kmax; k++)
        {
            Result[n] += Signal[k] * Kernel[n - k];
        }
    }
}


jstring
Java_com_example_xunda_lab1_1audio_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject this) {
    char* hello = "Hello from C";
    return (*env)->NewStringUTF(env, hello);
}

void Java_com_example_xunda_lab1_1audio_MainActivity_doProcessing(JNIEnv *env, jobject this) {
    double signal[SIGNAL_LIST_MAX];
    int count = 0;
    int i = 0;

    FILE *file;
    file = fopen(SIGNAL_FILE,"r");
    if(!file)    {
        perror("Error opening file");
        return;
    }


    memset(signal, 0, sizeof(signal));
    while (!feof(file)               /* Check for the end of file*/
           &&(count < SIGNAL_LIST_MAX)) /* To avoid memory corruption */
    {
        double temp_val = 0.0;
        fscanf(file, "%lf", &temp_val);
        signal[count++] = temp_val;
    }
    fclose(file);

    double carrier[SIGNAL_LIST_MAX];
    double kernel[FILTER_MAX];
    int c_count = 0;
    int j = 0;

    FILE *cfile;
    cfile = fopen("carrier.dat","r");
    if(!cfile)    {
        perror("Error opening file");
        return;
    }


    memset(carrier, 0, sizeof(signal));
    while (!feof(cfile)               /* Check for the end of file*/
           &&(c_count < SIGNAL_LIST_MAX)) /* To avoid memory corruption */
    {
        double ctemp_val = 0.0;
        fscanf(file, "%lf", &ctemp_val);
        carrier[c_count++] = ctemp_val;
        //printf("New val is : %lf\n", ctemp_val);
    }
    fclose(cfile);


    int k = 0;
    int f_count = 0;
    FILE *ffile;
    ffile = fopen("filt300.dat","r");
    if(!ffile)    {
        perror("Error opening file");
        return;
    }


    memset(kernel, 0, FILTER_MAX);
    while (!feof(ffile)               /* Check for the end of file*/
           &&(f_count < FILTER_MAX)) /* To avoid memory corruption */
    {
        double ftemp_val = 0.0;
        fscanf(ffile, "%lf", &ftemp_val);
        kernel[f_count++] = ftemp_val;
        printf("New val is : %lf\n", ftemp_val);
    }
    fclose(ffile);

    double result[ELEMENT_COUNT(signal) + ELEMENT_COUNT(kernel) - 1];

    convolve(signal, ELEMENT_COUNT(signal),
             kernel, ELEMENT_COUNT(kernel),
             result);

    FILE *outfile;
    outfile = fopen("result.txt", "w");
    if (!outfile) {
        perror("Error opening result file");
        return;
    }
    int wcount = 0;
    while (wcount < SIGNAL_LIST_MAX) {
        fprintf(outfile, "%lf\n", result[wcount]);
        wcount++;
    }
    fclose(outfile);
    //return;

    size_t mod_length = ELEMENT_COUNT(signal) + ELEMENT_COUNT(kernel) - 1;
    double mod_result[mod_length];

    double fs = 44100;
    double fc = 20000;
    double Tc = 1/fc;
    for (j=0; j<mod_length;j++) {
        mod_result[j] = (1+result[j])*carrier[j];
    }




    FILE *modfile;
    modfile = fopen("mod_result.txt", "w");
    if (!modfile) {
        perror("Error opening mod file");
        return;
    }
    int mwcount = 0;
    while (mwcount < SIGNAL_LIST_MAX) {
        fprintf(modfile, "%lf\n", mod_result[mwcount]);
        mwcount++;
    }
    fclose(modfile);
    return;


}

