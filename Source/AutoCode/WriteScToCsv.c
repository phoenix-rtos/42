#include "42.h"
#define EXTERN extern
#include "Ac.h"
#undef EXTERN

long ScTlmEnabled, MaxScCtr;
long ScBTlmEnabled, MaxScBCtr;
long ScGTlmEnabled, MaxScGCtr;
long ScWhlTlmEnabled, MaxScWhlCtr;
long ScGyroTlmEnabled, MaxScGyroCtr;
long ScShakerTlmEnabled, MaxScShakerCtr;

static FILE **ScOutfile = NULL;
static FILE **ScBOutfile = NULL;
static FILE **ScGOutfile = NULL;
static FILE **ScWhlOutfile = NULL;
static FILE **ScGyroOutfile = NULL;
static FILE **ScShakerOutfile = NULL;
/******************************************************************************/
void WriteScToCsv(void)
{
      struct SCType *S;
      char FileName[80];
      long Isc;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         ScOutfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               if (Nsc==1) sprintf(FileName,"Sc.csv");
               else if (Nsc<=10) sprintf(FileName,"Sc%1ld.csv",Isc);
               else sprintf(FileName,"Sc%02ld.csv",Isc);
               ScOutfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(ScOutfile[Isc],"Sc_Time");
               for(i=0;i<4;i++) {
                  fprintf(ScOutfile[Isc],",Sc_qn_%ld",i+1);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",Sc_wn_%ld",i+1);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",Sc_PosR_%ld",i+1);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",Sc_VelR_%ld",i+1);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",Sc_svb_%ld",i+1);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",Sc_bvb_%ld",i+1);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",Sc_Hvb_%ld",i+1);
               }
               fprintf(ScOutfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxScCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               fprintf(ScOutfile[Isc],"%18.12le",SimTime);
               for(i=0;i<4;i++) {
                  fprintf(ScOutfile[Isc],",%18.12le",S->qn[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",%18.12le",S->wn[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",%18.12le",S->PosR[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",%18.12le",S->VelR[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",%18.12le",S->svb[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",%18.12le",S->bvb[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(ScOutfile[Isc],",%18.12le",S->Hvb[i]);
               }
               fprintf(ScOutfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteScBToCsv(void)
{
      struct SCType *S;
      char FileName[80];
      long Isc;
      long k;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         ScBOutfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               if (Nsc==1) sprintf(FileName,"ScB.csv");
               else if (Nsc<=10) sprintf(FileName,"ScB%1ld.csv",Isc);
               else sprintf(FileName,"ScB%02ld.csv",Isc);
               ScBOutfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(ScBOutfile[Isc],"ScB_Time");
               for(k=0;k<S->Nb;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScBOutfile[Isc],",Sc_B%ld_wn_%ld",k+1,i+1);
                  }
               }
               for(k=0;k<S->Nb;k++) {
                  for(i=0;i<4;i++) {
                     fprintf(ScBOutfile[Isc],",Sc_B%ld_qn_%ld",k+1,i+1);
                  }
               }
               fprintf(ScBOutfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxScBCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               fprintf(ScBOutfile[Isc],"%18.12le",SimTime);
               for(k=0;k<S->Nb;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScBOutfile[Isc],",%18.12le",S->B[k].wn[i]);
                  }
               }
               for(k=0;k<S->Nb;k++) {
                  for(i=0;i<4;i++) {
                     fprintf(ScBOutfile[Isc],",%18.12le",S->B[k].qn[i]);
                  }
               }
               fprintf(ScBOutfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteScGToCsv(void)
{
      struct SCType *S;
      char FileName[80];
      long Isc;
      long k;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         ScGOutfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               if (Nsc==1) sprintf(FileName,"ScG.csv");
               else if (Nsc<=10) sprintf(FileName,"ScG%1ld.csv",Isc);
               else sprintf(FileName,"ScG%02ld.csv",Isc);
               ScGOutfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(ScGOutfile[Isc],"ScG_Time");
               for(k=0;k<S->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScGOutfile[Isc],",Sc_G%ld_Pos_%ld",k+1,i+1);
                  }
               }
               for(k=0;k<S->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScGOutfile[Isc],",Sc_G%ld_PosRate_%ld",k+1,i+1);
                  }
               }
               for(k=0;k<S->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScGOutfile[Isc],",Sc_G%ld_Ang_%ld",k+1,i+1);
                  }
               }
               for(k=0;k<S->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScGOutfile[Isc],",Sc_G%ld_AngRate_%ld",k+1,i+1);
                  }
               }
               fprintf(ScGOutfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxScGCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               fprintf(ScGOutfile[Isc],"%18.12le",SimTime);
               for(k=0;k<S->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScGOutfile[Isc],",%18.12le",S->G[k].Pos[i]);
                  }
               }
               for(k=0;k<S->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScGOutfile[Isc],",%18.12le",S->G[k].PosRate[i]);
                  }
               }
               for(k=0;k<S->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScGOutfile[Isc],",%18.12le",S->G[k].Ang[i]);
                  }
               }
               for(k=0;k<S->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(ScGOutfile[Isc],",%18.12le",S->G[k].AngRate[i]);
                  }
               }
               fprintf(ScGOutfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteScWhlToCsv(void)
{
      struct SCType *S;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         ScWhlOutfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               if (Nsc==1) sprintf(FileName,"ScWhl.csv");
               else if (Nsc<=10) sprintf(FileName,"ScWhl%1ld.csv",Isc);
               else sprintf(FileName,"ScWhl%02ld.csv",Isc);
               ScWhlOutfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(ScWhlOutfile[Isc],"ScWhl_Time");
               for(k=0;k<S->Nw;k++) {
                  fprintf(ScWhlOutfile[Isc],",Sc_Whl%ld_H",k+1);
               }
               fprintf(ScWhlOutfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxScWhlCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               fprintf(ScWhlOutfile[Isc],"%18.12le",SimTime);
               for(k=0;k<S->Nw;k++) {
                  fprintf(ScWhlOutfile[Isc],",%18.12le",S->Whl[k].H);
               }
               fprintf(ScWhlOutfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteScGyroToCsv(void)
{
      struct SCType *S;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         ScGyroOutfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               if (Nsc==1) sprintf(FileName,"ScGyro.csv");
               else if (Nsc<=10) sprintf(FileName,"ScGyro%1ld.csv",Isc);
               else sprintf(FileName,"ScGyro%02ld.csv",Isc);
               ScGyroOutfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(ScGyroOutfile[Isc],"ScGyro_Time");
               for(k=0;k<S->Ngyro;k++) {
                  fprintf(ScGyroOutfile[Isc],",Sc_Gyro%ld_TrueRate",k+1);
               }
               fprintf(ScGyroOutfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxScGyroCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               fprintf(ScGyroOutfile[Isc],"%18.12le",SimTime);
               for(k=0;k<S->Ngyro;k++) {
                  fprintf(ScGyroOutfile[Isc],",%18.12le",S->Gyro[k].TrueRate);
               }
               fprintf(ScGyroOutfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteScShakerToCsv(void)
{
      struct SCType *S;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         ScShakerOutfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               if (Nsc==1) sprintf(FileName,"ScShaker.csv");
               else if (Nsc<=10) sprintf(FileName,"ScShaker%1ld.csv",Isc);
               else sprintf(FileName,"ScShaker%02ld.csv",Isc);
               ScShakerOutfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(ScShakerOutfile[Isc],"ScShaker_Time");
               for(k=0;k<S->Nsh;k++) {
                  fprintf(ScShakerOutfile[Isc],",Sc_Shaker%ld_Output",k+1);
               }
               fprintf(ScShakerOutfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxScShakerCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               S = &SC[Isc];
               fprintf(ScShakerOutfile[Isc],"%18.12le",SimTime);
               for(k=0;k<S->Nsh;k++) {
                  fprintf(ScShakerOutfile[Isc],",%18.12le",S->Shaker[k].Output);
               }
               fprintf(ScShakerOutfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteScVarsToCsv(void)
{
      FILE *infile;
      char response[80],junk[80],newline;
      double dt;
      static long First = 1;

      if (First) {
         First = 0;

         infile = FileOpen(InOutPath,"Inp_ScOutput.txt","r");
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         ScTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("Sc timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxScCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         ScBTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("ScB timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxScBCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         ScGTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("ScG timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxScGCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         ScWhlTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("ScWhl timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxScWhlCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         ScGyroTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("ScGyro timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxScGyroCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         ScShakerTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("ScShaker timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxScShakerCtr = (long) (dt/DTSIM+0.5);

      }

      if (ScTlmEnabled) WriteScToCsv();
      if (ScBTlmEnabled) WriteScBToCsv();
      if (ScGTlmEnabled) WriteScGToCsv();
      if (ScWhlTlmEnabled) WriteScWhlToCsv();
      if (ScGyroTlmEnabled) WriteScGyroToCsv();
      if (ScShakerTlmEnabled) WriteScShakerToCsv();

}

void CloseScCsvFiles(void)
{
   FileClose(ScOutfile, Nsc);
   free(ScOutfile);
   FileClose(ScBOutfile, Nsc);
   free(ScBOutfile);
   FileClose(ScGOutfile, Nsc);
   free(ScGOutfile);
   FileClose(ScWhlOutfile, Nsc);
   free(ScWhlOutfile);
   FileClose(ScGyroOutfile, Nsc);
   free(ScGyroOutfile);
   FileClose(ScShakerOutfile, Nsc);
   free(ScShakerOutfile);
}
