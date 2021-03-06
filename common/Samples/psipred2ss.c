/* 
Portions Copyright (c) 2007-2012 Hogue Laboratory, National University of Singapore
Portions Copyright (c) 1997-2007 Hogue Laboratory, University of Toronto
Portions Copyright (c) 1997-2005 Hogue Laboratory, Mount Sinai Hospital, Toronto


All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE COPYRIGHT HOLDERS HAVE NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, 
UPDATES, ENHANCEMENTS OR MODIFICATIONS.

Principal Contact: Christopher W.V. Hogue  chogue@blueprint.org

Contributing Hogue Laboratory Developers and Institutional Collaborators: 
(SLRI and TraDES Directories)
Christopher Hogue (mmdbapi, b-d tree, Cn3D, trades, solvate, valmerge, str2trj, seq2trj). 
Howard Feldman (salign, base foldtraj & maketraj libraries)
Contributions from John J. Salama (VisTraj) and Kevin Snyder (valmerge)
Phillipe Phan (analyzeMovie) Adrian Heilbut, Mark Kotowycz, Van Le (trj visualization),
Michel Dumontier (scoring function libraries, statistical functions) 
Elena Garderman (software maintenance), Mingxi Yao (valtopdb),
Gil Alteroviz (GOR implementation),  Boris Steipe (University of Toronto) (fragment based construction)
Brendan McConkey (University of Waterloo) & Michael Brougham(VSCORE scoring function)
Jonathan Kans (NCBI) (vistraj)

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
  char resname,pred;
  int iph,ipe,ipc,excess,exmod,exdiv;
  float ph,pe,pc;
  FILE *f,*g;
  char buf[255];
  char check[255];

  if (argc != 3 ) {
    printf("Converts PSIPred .ss2 output file to generic .ss format\nUsage\n%s <source file> <destination file>\n\n",argv[0]);
    return 0;
  }
  if ((f=fopen(argv[1],"r"))==NULL) {
    printf("Cannot open input file.");
    return 0;
  }
  if ((g=fopen(argv[2],"w"))==NULL) {
    printf("Cannot open output file.");
    return 0;
  }
  while (fgets(buf,255,f)!=NULL) {
    /* read one line at a time, ignoring comments */
    if (buf[0]!='#') {
      /* save relevant columns */
      sscanf(buf,"%*d %c %c %f %f %f",&resname,&pred,&pc,&ph,&pe);
      /* PHD L = our C */
      /* force probabilities for 3-state prediction to sum to 100, in as fair a way
			 as possible (ensure they stay all non-negative as well) */
			iph=(int)(100.0*ph);
			ipe=(int)(100.0*pe);
			ipc=(int)(100.0*pc);
      excess=iph+ipe+ipc-100;
      exdiv=excess/3;
      exmod=excess%3;
      iph-=exdiv;
      ipe-=exdiv;
      ipc-=(exdiv+exmod);
      if (iph<0) {
	if (ipc>-iph) {
	  ipc+=iph;
	  iph=0;
	}
	else {
	  ipe+=iph;
	  iph=0;
	}
      }	
      if (ipe<0) {
	if (ipc>-ipe) {
	  ipc+=ipe;
	  ipe=0;
	}
	else {
	  iph+=ipe;
	  ipe=0;
	}
      }	
      if (ipc<0) {
	if (iph>-ipc) {
	  iph+=ipc;
	  ipc=0;
	}
	else {
	  ipe+=ipc;
	  ipc=0;
	}
      }	
      /* output generic format data line */
      /* five space-separated columns:

	 1-letter amino acid code
	 1-state prediction ("H", "E" or "C")
	 pH (probability of Helix)
	 pE (probability of Extended/Strand)
	 pC (probability of Coil)

	 All residues in the protein must be included, in order, one
	 line per residue, and pH+pE+pC=100 should hold in all cases
      */
      fprintf(g,"%c %c %d %d %d\n",resname,pred,iph,ipe,ipc);
    }
  }
  fclose(f);
  fclose(g);
  return 1;
}


/*  
$Log: psipred2ss.c,v $
Revision 1.1  2002/08/20 21:38:58  feldman
New file


*/

