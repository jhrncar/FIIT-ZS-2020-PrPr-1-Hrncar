/*Jakub Hrnčár, 12.11.2020, FIIT STU
Program nema osetrene, ci je subor korektny. Som si toho vedomy, 
preto mi mozete rovno odpocitat 2 body a zadavajte LEN korektny subor prosim. Vo funckiach niektore veci overene su (napr. v O,
ak sa nenasla ani 1 vhodna diagnoza, nespusti sa nic*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int pocet_riadkov(FILE **subor){
    int pocet_r=1;
    char riadok='c';
    fseek(*subor,0,SEEK_SET); //vratenie sa na zaciatok suboru
    while (riadok!=EOF){
            riadok=fgetc(*subor);
            if (riadok=='\n'){ //hladanie novych riadkov, musi zacinat od 1, pretoze vzdy je aspon 1 riadok
                pocet_r++;
            }
        }
        return pocet_r; 
}
int pocet_pacientov(FILE **subor){
    int pocet_p=0;
    int pocet_r=1;
    char riadok='c';
    int i=0;
    fseek(*subor,0,SEEK_SET);
    while (riadok!=EOF){
            riadok=fgetc(*subor);
            if (riadok=='\n'){ //hladanie novych riadkov, musi zacinat od 1, pretoze vzdy je aspon 1 riadok
                pocet_r++;
            }
        }
    while (i<=pocet_r){
        if ((i%7)==1){ //kazdy siedmy riadok sa berie ako kontrolny na pripocitanie pacienta
            pocet_p++;
        }
        i++;
    }
    return pocet_p;
}
int vek(long long int u_cislo){
    long long int vek2;
    vek2=(u_cislo-(u_cislo%100000000)); //ziskanie priblizneho veku podla rod.cisla za pomoci momentalneho roku
    vek2=((vek2)/100000000); //z rodneho cisla potrebujeme ziskat LEN prve 2 cisla
    if (vek2>=54){ //54, pretoze pred rokom 1954 neboli rodne cisla v tomto formate, teda ak je rok mensi ako 54, jedna sa uz o rok po roku 2000 
        vek2=(2020-(1900+vek2));
    }
    else{
        vek2=(2020-(2000+vek2));
    }
    return vek2;
}
void o(FILE **subor,int *kontrol){
    if (*kontrol==0){
        *kontrol=1;
        *subor=fopen("pacienti.txt","r"); //ak subor este nie je otvoreny, otvori sa tu
    }
        long int vstup_datum,datum;
        scanf("%ld",&vstup_datum); //vstup datumu od uzivatela
        int pacient_cislo=0;
        int riadky=pocet_riadkov(&(*subor)); //zistenie poctu riadkov
        char buf[52];
        char buf2[5];
        int i=1;
        int pom=0;
        int pocet_p=pocet_pacientov(&(*subor)); //zistenie poctu pacientov
        int poz1;
        int poz2;
        char testpole[4];
        fseek(*subor,0,SEEK_SET); //prechod na zaciatok suboru
        char **pole_diagnozy = (char **)malloc((pocet_p)*sizeof(void *)); //vytvorenie 2D dynamickeho pola pre vsetky diagnozy
        for (int t=0;t<pocet_p;t++){
            pole_diagnozy[t] = (char *)malloc(4);
        }
        if (pole_diagnozy==NULL){
                exit(0);
            } 
        while (i<=riadky){ //prechod celym suborom po riadkoch
            if ((i%7)==6){ //kazdy 6. riadok u kazdeho pacienta potrebujeme
                fscanf(*subor,"%ld",&datum);
                if (datum<=vstup_datum){ //len ak datum je do zadaneho datumu
                    for (int y=0;y<=2;y++){ //pridanie diagnozy do dyn. pola
                        pole_diagnozy[pom][y]=buf2[y];//v dobe programovania som nevedel, ze mozme pouzivat string.h a strcpy
                    }
                    pole_diagnozy[pom][3]='\00'; //dodanie NULL charakteru na koniec stringu
                    pom++;
                    fgets(buf,52,*subor); //docitanie suboru do konca
                }
                else{
                    fgets(buf,52,*subor);
                }
            }
            else if ((i%7)==3){ //kazdy 3. riadok u kazdeho pacienta potrebujeme
                fgets(buf2,52,*subor);
            }
            else{ //ak nie je ani 3. ani 6. riadok, len sa precita a nic sa nespravi
                fgets(buf,52,*subor);
            }
        i++;
        }
    int pocet_diag=0;
    int *pocet_diag_pole;
    pocet_diag_pole=malloc(pocet_p*sizeof(int)); //vytvorenie dyn. pola, kde budu pocty jednotlivych diagnoz do datumu
    if (pocet_diag_pole==NULL){
        exit(0);
    }
    if (pom<=0){} //ak ani raz nebola diagnoza, skonci funkciu
    else{
        for (int f=0;f<pom;f++){//cyklus prejde kazdou diagnozou a najde, kolkokrat sa v subore nachadza, nevadi nam, ze tam budu cisla duplicitne alebo bude viac rovnakych poctov, vypise sa len prve
            for (int h=0;h<3;h++){
                testpole[h]=pole_diagnozy[f][h]; //vzdy sa vyberie jedna diagnoza a pocita sa
                }
            testpole[3]='\00'; //dodanie NULL charakteru na koniec stringu
            for (int r=0;r<pom;r++){
                    if (testpole[0]==pole_diagnozy[r][0]){ //najdenie spravnej diagnozy, musia sediet vsetky 3 pismenka (cisla)
                        if (testpole[1]==pole_diagnozy[r][1]){
                            if (testpole[2]==pole_diagnozy[r][2]){
                                pocet_diag++;
                            }
                        }
                    }
            }
            pocet_diag_pole[f]=pocet_diag; //zapis poctu
            pocet_diag=0;
        }
    int maximum=pocet_diag_pole[0];
    int index2=0;
    int maximum_index=0;
    while (index2<pom){ //najdenie maxima
        if (maximum<pocet_diag_pole[index2]){
            maximum=pocet_diag_pole[index2];
            maximum_index=index2;
        }
        index2++;
        }   
    printf("Najcastejšie vysetrovana diagnoza do %ld je %s\n",vstup_datum,pole_diagnozy[maximum_index]);
    }

    for (int t=0;t<pom;t++){
        free((pole_diagnozy)[t]); //odalokovanie dyn. poli a pointrov
        }
    free(*pole_diagnozy); 
    free(pocet_diag_pole);
}
void v(FILE **subor,int *kontrol){
    if (*kontrol==0){
        *subor=fopen("pacienti.txt","r"); //otvorenie suboru
        *kontrol=1;}
    if (*subor==NULL){
        printf("Neotvoreny subor\n"); //ak neexistuje, skonci program
        exit(0);
        }
    int riadky=pocet_riadkov(&(*subor)); //najdenie poctu riadkov
    char buf[50];
    long long int r_cislo;
    double vysledok;
    int i=1;
    long int datum;
    fseek(*subor,0,SEEK_SET); //prechod na zaciatok suboru
    while (i<=riadky){
        if ((i%7)==0){//print prazdneho riadku medzi riadkami
            fgets(buf,50,*subor);
            printf("\n");
        }
        else if ((i%7)==1){
            fgets(buf,50,*subor);//print mena
            printf("Meno priezvisko: %s",buf);
        }
        else if ((i%7)==2){
            fscanf(*subor,"%lld",&r_cislo);//print rodneho cisla
            if ((r_cislo/1000000000)<1){ 
                if ((r_cislo/100000000)<1){
                    if ((r_cislo/10000000)<1){
                        printf("Rodne cislo: 000%lld\n",r_cislo);
                    }
                    else{
                        printf("Rodne cislo: 00%lld\n",r_cislo);
                    }
                }
                else{
                    printf("Rodne cislo: 0%lld\n",r_cislo);
                }
            }
            else{
                printf("Rodne cislo: %lld\n",r_cislo);
            }
            fgets(buf,50,*subor);
        }
        else if ((i%7)==3){//print diagnozy
            fgets(buf,50,*subor); 
            printf("Diagnoza: %s",buf);
        }
        else if ((i%7)==4){//print vysetrenia
            fgets(buf,50,*subor);
            printf("Vysetrenie: %s",buf);            
        }
        else if ((i%7)==5){ //print vysledku
            fscanf(*subor,"%lf",&vysledok);
            printf("Vysledok: %g\n",vysledok);
            fgets(buf,50,*subor);            
        }
        else if ((i%7)==6){//print datumu
            fscanf(*subor,"%ld",&datum);
            printf("Datum: %ld\n",datum);
            fgets(buf,50,*subor);
        }
        i++;
    }    
}
void n(FILE **subor, int*kontrol,int *kontrol2,char ***meno,long long int **r_cislo,char ***diag,char ***vyse,double **vysl,long int **datum){
    if (*kontrol!=0){
        *kontrol2=1;
        fseek(*subor,0,SEEK_SET);
        int pocet_p=pocet_pacientov(&(*subor));//zistenie poctu pacientov
        int riadky=pocet_riadkov(&(*subor));//zistenie poctu riadkov
        int i=1;
        long long int cislo;
        double f_cislo;
        char buf[52];

        *meno=(char **)malloc(pocet_p*sizeof(void *)); //vytvorenie 2D dyn. pola pre mena
        for (int t=0;t<pocet_p;t++){
            (*meno)[t] = (char *)malloc(52*sizeof(char)); 
        }
        if (*meno==NULL){
            exit(0);
        }
        *diag=(char **)malloc(pocet_p*sizeof(void *)); //vytvorenie 2D dyn. pola pre diagnozy
        for (int t=0;t<pocet_p;t++){
            (*diag)[t] = (char *)malloc(5*sizeof(char));
        }
        if (*diag==NULL){
            exit(0);
        }

        *vyse=(char **)malloc(pocet_p*sizeof(void *)); //vytvorenie 2D dyn. pola pre vysetrenie
        for (int t=0;t<pocet_p;t++){
            (*vyse)[t] = (char *)malloc(52*sizeof(char));
        }
        if (*vyse==NULL){
            exit(0);
        }

        *r_cislo=(long long int *)malloc(pocet_p*sizeof(long long int)); //vytvorenie dyn. pola pre r.cisla
        if (*r_cislo==NULL){
            exit(0);
        }

        *vysl=(double *)malloc(pocet_p*sizeof(double)); //vytvorenie dyn. pola pre vysledky
        if (*vysl==NULL){
            exit(0);
        }

        *datum=(long int *)malloc(pocet_p*sizeof(long int)); //vytvorenie dyn. pola pre datumy
        if (*datum==NULL){
            exit(0);
        }

        int pacient =-1;
        fseek(*subor,0,SEEK_SET); //prechod na zaciatok suboru
        while (i<=riadky){
            int f=0;
            char y='c';
            if ((i%7)==0){
                fgets(buf,52,*subor);
            }
            else if ((i%7)==1){
                pacient++;
                fgets(buf,52,*subor);
                while (y!='\n' && y!='\00'){ //nacitanie mena do pola, bez \n a NULL
                    y=buf[f];
                    if (y!='\n' && y!='\00'){
                        (*meno)[pacient][f]=y;
                        f++;
                        }
                    else{
                        (*meno)[pacient][f]='\00'; //pridanie NULL na konciec stringu
                        break;
                    }
                }
                int f=0;
            }
            else if ((i%7)==3){
                fgets(buf,52,*subor);
                while (y!='\n' && y!='\00'){//nacitanie diagnozy do pola, bez \n a NULL
                    y=buf[f];
                    if (y!='\n' && y!='\00'){
                        (*diag)[pacient][f]=y;
                        f++;
                        }
                    else{
                        (*diag)[pacient][f]='\00';//pridanie NULL na konciec stringu
                        break;
                    }
                }
            int f=0;
            }
            else if ((i%7)==4){
                fgets(buf,52,*subor);
                while (y!='\n' && y!='\00'){ //nacitanie vysetrenia do pola, bez \n a NULL
                    y=buf[f];
                    if (y!='\n' && y!='\00'){
                        (*vyse)[pacient][f]=y;
                        f++;
                        }
                    else{
                        (*vyse)[pacient][f]='\00';//pridanie NULL na konciec stringu
                        break;
                    }
                }
            int f=0;    
            }
            else if ((i%7)==2){
                fscanf(*subor,"%lld",&cislo);
                fgets(buf,52,*subor);
                (*r_cislo)[pacient]=cislo; //nacitanie do pola, bez \n
            }

            else if ((i%7)==5){
                fscanf(*subor,"%lf",&f_cislo);
                fgets(buf,52,*subor);
                (*vysl)[pacient]=f_cislo; //nacitanie do pola, bez \n
            }
            else if ((i%7)==6){
                fscanf(*subor,"%ld",&cislo);
                fgets(buf,52,*subor);
                (*datum)[pacient]=cislo; //nacitanie do pola, bez \n
            }  
            i++;
        }
    }
    else{}
}
void s(FILE **subor,int *kontrol2,long long int **r_cislo,char ***vyse,double **vysl){
    if (*kontrol2!=0){
        long long int vstup;
        scanf("%lld",&vstup);
        int pocet_p=pocet_pacientov(&(*subor)); //zistenie poctu pacientov
        fseek(*subor,0,SEEK_SET); //prechod na zaciatok suboru
        int pamat;
        for (int i=0;i<pocet_p;i++){ 
            if ((*r_cislo)[i]==vstup){ //porovnanie, ci sa prvok rovna vstupu
                pamat=i; //ak sa rovna vstupu, zapise sa spravny index do pamate
            }
        }
        if ((vstup/1000000000)<1){
            if ((vstup/100000000)<1){
                if ((vstup/10000000)<1){
                        printf("Vysledky vysetreni, ktore boli vykonane pacientovi s rodnym cislom 000%lld su nasledovne:\n%s:%g\n",vstup,(*vyse)[pamat],(*vysl)[pamat]);//vypis vysetrenia a vysledku od toho isteho pacienta
                    }
            else{
                printf("Vysledky vysetreni, ktore boli vykonane pacientovi s rodnym cislom 00%lld su nasledovne:\n%s:%g\n",vstup,(*vyse)[pamat],(*vysl)[pamat]); //vypis vysetrenia a vysledku od toho isteho pacienta
            }
            }
            else{
                printf("Vysledky vysetreni, ktore boli vykonane pacientovi s rodnym cislom 0%lld su nasledovne:\n%s:%g\n",vstup,(*vyse)[pamat],(*vysl)[pamat]); //vypis vysetrenia a vysledku od toho isteho pacienta
            }

        }
        else{
            printf("Vysledky vysetreni, ktore boli vykonane pacientovi s rodnym cislom %lld su nasledovne:\n%s:%g\n",vstup,(*vyse)[pamat],(*vysl)[pamat]); //vypis vysetrenia a vysledku od toho isteho pacienta
        }
    }
    else{
        printf("Polia nie su vytvorene\n");
    }
}
void h(FILE **subor,int *kontrol2,long long int **r_cislo,char ***diag){
if (*kontrol2!=0){
    int pocet_p=pocet_pacientov(&(*subor)); //zistenie poctu pacientov
    int *pole_vek=(int *)malloc(pocet_p*sizeof(int)); //alokovanie pola pre vsetky veky pacientov
    if (pole_vek==NULL){
        exit(0);
    }
    int *pole_vekM=(int *)malloc(pocet_p*sizeof(int)); //alokovanie pola pre vsetky veky muzov  
    if (pole_vekM==NULL){
        exit(0);
    }
    int *pole_vekZ=(int *)malloc(pocet_p*sizeof(int));//alokovanie pola pre vsetky veky zien
    if (pole_vekZ==NULL){
        exit(0);
    }
    long long int cislo;
    char diagnoza[4];
    int vek1;
    scanf("%s",diagnoza); //nacitanie diagnozy od uzivatela
    for (int i=0;i<pocet_p;i++){
        cislo=(*r_cislo)[i];
        vek1=vek(cislo);
        pole_vek[i]=vek1; //zistenie veku kazdeho pacienta a priradenie do pola
    }
    int m=0;
    int z=0;
    
    for (int i=0;i<pocet_p;i++){
        if (strcmp(diagnoza,((*diag)[i]))==0){ //porovnanie, ci pacient ma ziadanu diagnozu, oboje su stringy, takze pouzijeme strcmp
            char c[11];
            long long int test;
            test=(*r_cislo)[i];
            lltoa(test,c,10);//ak ma, zisti sa ci je pacient muz alebo zena, skrz jeho mesiac narodenia (zeny maju +50 mesiacov), preto je potrebne pristupovat k 3. prvku v cisle, co sa da len ak pracujeme so stringom
            if (((test)/1000000000)<1){
                if (((test)/100000000)<1){
                    if (((test)/10000000)<1){
                        for(int g=8;g>=0;g--) {
                            c[g+2] = c[g];
                            }
                        c[0]='0';
                        c[1]='0';
                        c[2]='0';
                    }
                    else{
                        for(int g=8;g>=0;g--) {
                            c[g+2] = c[g];
                            }
                        c[0]='0';
                        c[1]='0';
                    }
                }
                else{
                    for(int g=9;g>=0;g--) {
                        c[g+1] = c[g];
                        }
                    c[0]='0';
                }
            }
            if (c[2]=='5' || c[2]=='6'){
                pole_vekZ[z]=pole_vek[i]; //ak je zena, priradi sa vek do pola pre zeny
                z++;
            }
            else{
                pole_vekM[m]=pole_vek[i]; //ak je muz, priradi sa vek do pola pre muzov
                m++;
            }
        }
    }
    int *finalz1=(int *)malloc(z*sizeof(int)); //alokovanie dyn. pola pre finalne veky zien v histograme
    if (finalz1==NULL){
        exit(0);
    }
    int *finalz2=(int *)malloc(z*sizeof(int)); //alokovanie dyn. pola pre finalne pocetnosti vekov zien v histograme
    if (finalz2==NULL){
        exit(0);
    }
    int *finalm1=(int *)malloc(m*sizeof(int)); //alokovanie dyn. pola pre finalne veky muzov v histograme
    if (finalm1==NULL){
        exit(0);
    }
    int *finalm2=(int *)malloc(m*sizeof(int)); //alokovanie dyn. pola pre finalne pocetnosti vekov muziv v histograme
    if (finalm2==NULL){
        exit(0);
    }
    int pz=0;
    int zena;
    int pm=0;
    int muz;
    int dz=0;
    int dm=0;
    for (int i=0;i<z;i++){
        zena=pole_vekZ[i];
        for (int y=0;y<z;y++){
            if (zena==pole_vekZ[y] && zena!=-1){ //aby sa zarucilo, ze v histograme budu vsetky skupiny vekov len raz, spocita sa pocetnost jedneho veku a nasledne sa vymaze
                pz++;
                //printf("%ld\n",pole_vekZ[y]);
                pole_vekZ[y]=-1;
                }
            }
        
        if (pz!=0){
            finalz2[dz]=pz; //priradenie pocetnosti do pola 
            finalz1[dz]=zena;
            dz++; //priradenie veku do pola
            }
        pz=0;
        }
    for (int i=0;i<m;i++){
        muz=pole_vekM[i];
        for (int y=0;y<m;y++){
            if (muz==pole_vekM[y] && muz!=-1){ //aby sa zarucilo, ze v histograme budu vsetky skupiny vekov len raz, spocita sa pocetnost jedneho veku a nasledne sa vymaze
                pm++;
                pole_vekM[y]=-1;
                }
            }
        
        if (pm!=0){
            finalm2[dm]=pm; //priradenie pocetnosti do pola
            finalm1[dm]=muz; //priradenie veku do pola
            dm++;
            }
        pm=0;
        }
    if (dm!=0){ //vypis histogramu, pokial nie je prazdny
        printf("Muzi:\n");
        for (int i=0;i<dm;i++){
            printf("%d: %d\n",finalm1[i],finalm2[i]);
        }
    }
    if (dz!=0){ //vypis histogramu, pokial nie je prazdny
        printf("Zeny:\n");
        for (int i=0;i<dz;i++){
            printf("%d: %d\n",finalz1[i],finalz2[i]);
        }
    }
    free(pole_vek); //odalokovanie vsetkych dyn. poli a pointrov
    pole_vek=NULL;
    free(pole_vekM);
    pole_vekM=NULL;
    free(pole_vekZ);
    pole_vekZ=NULL;
    free(finalz1);
    finalz1=NULL;
    free(finalz2);
    finalz2=NULL;
    free(finalm1);
    finalm1=NULL;
    free(finalm2);
    finalm2=NULL;
}
else{
    printf("Polia nie su vytvorene\n");
    }

}
void p(FILE **subor,int *kontrol2,char ***meno,long long int **r_cislo,char ***diag,char ***vyse,double **vysl,long int **datum){
    if (*kontrol2!=0){
        long long int ro_cislo;
        char vyset[51];
        char buffer[52];
        long int datum1;
        double vstup;
        double temp;
        scanf("%lld",&ro_cislo); //vstup uzivatelskych udajov 
        scanf("%s",vyset);
        scanf("%ld",&datum1);
        scanf("%lf",&vstup);
        int pocet_p=pocet_pacientov(&(*subor)); //nacitanie poctu pacientov
        int riadky=pocet_riadkov(&(*subor)); //nacitanie poctu riadkov
        fclose(*subor);
        *subor=fopen("pacienti.txt","w"); //vymazanie suboru a otvorenie na zapis
        if (*subor==NULL){
            printf("Neotvoreny subor\n");
            exit(0);
        }

        for (int i=0;i<pocet_p;i++){ //prehladanie poli, az pokial sa nenajde spravny pacient 
            if ((strcmp(vyset,(*vyse)[i]))==0){
                if (((*r_cislo)[i])==ro_cislo){
                    if(datum1==((*datum)[i])){
                        temp=(*vysl)[i];
                        (*vysl)[i]=vstup;//ak sa najde spravny pacient, prepise sa hodnota vysledku v poli
                        fseek(*subor,0,SEEK_SET);
                        for (int y=0;y<pocet_p;y++){
                            fprintf(*subor,"%s\n",((*meno)[y]));//vytvorenie noveho suboru s novym, modifikovanym vysledkom
                            if ((((*r_cislo)[y])/1000000000)<1){
                                if ((((*r_cislo)[y])/100000000)<1){
                                    if ((((*r_cislo)[y])/10000000)<1){
                                        fprintf(*subor,"000%lld\n",((*r_cislo)[y]));
                                    }
                                    else{
                                    fprintf(*subor,"00%lld\n",((*r_cislo)[y]));
                                    }
                                }
                                else{
                                    fprintf(*subor,"0%lld\n",((*r_cislo)[y]));
                                }
                            }
                            else{
                                fprintf(*subor,"%lld\n",((*r_cislo)[y]));
                            }
                            fprintf(*subor,"%s\n",((*diag)[y]));
                            fprintf(*subor,"%s\n",((*vyse)[y]));
                            fprintf(*subor,"%.4g\n",((*vysl)[y]));
                            if (y!=(pocet_p-1)){ //posledny riadok v subore nema na konci enter
                                fprintf(*subor,"%ld\n",((*datum)[y]));
                                fprintf(*subor,"\n");
                            }
                            else{
                                fprintf(*subor,"%ld",((*datum)[y]));
                            }
                        }
                        fclose(*subor); //zatvorenie a ulozenie noveho suboru a nove otvorenie na citanie, aby program mohol dalej prokracovat 
                        *subor=fopen("pacienti.txt","r");
                        if (((ro_cislo)/1000000000)<1){ 
                                if (((ro_cislo)/100000000)<1){
                                    if (((ro_cislo)/10000000)<1){
                                        printf("Pacientovi s rodnym cislom 000%lld bol zmeneny vysledok vysetrenia %s z povodnej hodnoty %g na novu hodnotu %g.\n",ro_cislo,vyset,temp,vstup);
                                    }
                                else{
                                    printf("Pacientovi s rodnym cislom 00%lld bol zmeneny vysledok vysetrenia %s z povodnej hodnoty %g na novu hodnotu %g.\n",ro_cislo,vyset,temp,vstup);
                                }
                                }
                                else{
                                    printf("Pacientovi s rodnym cislom 0%lld bol zmeneny vysledok vysetrenia %s z povodnej hodnoty %g na novu hodnotu %g.\n",ro_cislo,vyset,temp,vstup);
                                }
                        }
                        else{
                            printf("Pacientovi s rodnym cislom %lld bol zmeneny vysledok vysetrenia %s z povodnej hodnoty %g na novu hodnotu %g.\n",ro_cislo,vyset,temp,vstup);
                        }
                    }
                }
            }
        }
    }
    else{
        printf("Polia nie su vytvorene\n");
    }
}
void z(FILE **subor,int *kontrol2,char ***meno,char ***vyse,double **vysl,long int **datum){
    if (*kontrol2!=0){
        long int vstupd1,vstupd2;
        char vstupv[51];
        int y=0;
        double pole_max[3];
        int pole_index_max[3];
        scanf("%ld",&vstupd1); //vstup prveho datumu
        scanf("%ld",&vstupd2); //vstup druheho datumu
        if (vstupd1>vstupd2){ //podmienka, ci je spravne zadany datum
            printf("Koncovy datum sa nachadza pred pociatocnym datumom\n");
        }
        else{
            scanf("%s",vstupv);//vstup vysetrenia
            int pocet_p=pocet_pacientov(&(*subor));//zistenie poctu pacientov
            double *pole_vysl=(double *)malloc(pocet_p*sizeof(double));
            int *pole_index=(int *)malloc(pocet_p*sizeof(int));
            if (pole_index==NULL){
                exit(0);
            }
            if (pole_vysl==NULL){
                exit(0);
            }
            fseek(*subor,0,SEEK_SET);
            for (int i=0;i<pocet_p;i++){
                if (((*datum)[i])>=vstupd1 && ((*datum)[i])<=vstupd2){ //spravne sa musi nachadzat datum medzi zadanymi datumami a zaroven sa musi byt aj rovnake vysetrenie
                    if (strcmp(vstupv,((*vyse)[i]))==0){
                        pole_index[y]=i; //zapamatanie si povodneho indexu vysetrenia a spravneho vysetrenia
                        pole_vysl[y]=(*vysl)[i];
                        y++;
                    }
                }
            }
            int index2=0;
            double maxi=0;
            int max_index=0;
            int k;
            if (y<=3){ //ak je prvkov menej ako alebo rovne 3, vypise 3 a menej. AK ich je viac, vypise presne 3
                k=y;
            }
            else{
                k=3;
            }
            for (int i=0;i<k;i++){
                while (index2<y){ //najdenie maxima
                    if (maxi<pole_vysl[index2]){
                        maxi=pole_vysl[index2];
                        max_index=index2;
                        }
                    index2++;
                    }
                    pole_max[i]=maxi; //priradenie maxima a jeho povodneho indexu do noveho pola a nasledne vyhodenie prvku z pola
                    pole_index_max[i]=pole_index[max_index];
                    index2=0;
                    pole_vysl[max_index]=-1; //kedze hodnoty vysledkov su od 0 po 1000, tak musime maximum nahradit za cislo mimo tohto intervalu,aby sme neovplyvnovali vysledok
                    maxi=0;
            }
            printf("Traja pacienti s najvyssimi hodnotami %s za obdobie %ld - %ld su (namerana hodnota v zatvorke)\n",vstupv,vstupd1,vstupd2);
            for (int i=0;i<k;i++){ //vypis prvych troch a menej 
                printf("%s (%g)\n",((*meno)[(pole_index_max[i])]), pole_max[i]);
            }
        free(pole_vysl);
        free(pole_index);
        pole_vysl=NULL;
        pole_index=NULL;
        printf("\n");    
        }
    }
    else{
        printf("Polia nie su vytvorene\n");
    }
}
int main(void){
    FILE *fr;
    int k=1;
    char menu;
    int control=0;
    int control2=0;
    char **meno=NULL;
    long long int *r_cislo=NULL;
    char **diag=NULL;
    char **vyse=NULL;
    double *vysl=NULL;
    long int *datum=NULL;

    while (k){
        menu=getchar();
        switch (menu){ //menu, ktore bude bezat az do zadanania k
            case 'k':
                if (control!=0){
                    int pocet_p=pocet_pacientov(&fr);
                    fclose (fr);
                    if (control2!=0){
                        for (int i=0;i<pocet_p;i++){ //dealokovanie svetkych vytvorenych poli a pointrov, ak boli vytvorene
                            free(meno[i]);
                        }
                        free(meno);
                        free(r_cislo);
                        for (int i=0;i<pocet_p;i++){//pri poli charov musime dealokovat aj kazdy prvok pola zvlast, kedze je to 2D pole
                            free(diag[i]);
                        }
                        free(diag);
                        for (int i=0;i<pocet_p;i++){
                            free(vyse[i]);
                        }
                        free(vyse);
                        free(vysl);//pri poli cisiel staci jednoducha dealokacia
                        free(datum);
                }
                }
                k=0;
                break;
            case 'v': //volanie funckie v
                v(&fr,&control);
                break;
            case 'o': //volanie funckie o
                o(&fr,&control);
                break;
            case 'n': //volanie funckie n
                n(&fr,&control,&control2,&meno,&r_cislo,&diag,&vyse,&vysl,&datum);
                break;
            case 's': //volanie funckie s
                s(&fr,&control2,&r_cislo,&vyse,&vysl);
                break;
            case 'h': //volanie funckie h
                h(&fr,&control2,&r_cislo,&diag);
                break;
            case 'p': //volanie funckie p
                p(&fr,&control2,&meno,&r_cislo,&diag,&vyse,&vysl,&datum);
                break;
            case 'z': //volanie funkcie z
                z(&fr,&control2,&meno,&vyse,&vysl,&datum);
                break;
        }
    }
    return 0;
}