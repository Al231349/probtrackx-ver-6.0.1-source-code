/*  Copyright (C) 2004 University of Oxford  */

/*  Part of FSL - FMRIB's Software Library
    http://www.fmrib.ox.ac.uk/fsl
    fsl@fmrib.ox.ac.uk

    Developed at FMRIB (Oxford Centre for Functional Magnetic Resonance
    Imaging of the Brain), Department of Clinical Neurology, Oxford
    University, Oxford, UK


    LICENCE

    FMRIB Software Library, Release 6.0 (c) 2018, The University of
    Oxford (the "Software")

    The Software remains the property of the Oxford University Innovation
    ("the University").

    The Software is distributed "AS IS" under this Licence solely for
    non-commercial use in the hope that it will be useful, but in order
    that the University as a charitable foundation protects its assets for
    the benefit of its educational and research purposes, the University
    makes clear that no condition is made or to be implied, nor is any
    warranty given or to be implied, as to the accuracy of the Software,
    or that it will be suitable for any particular purpose or for use
    under any specific conditions. Furthermore, the University disclaims
    all responsibility for the use which is made of the Software. It
    further disclaims any liability for the outcomes arising from using
    the Software.

    The Licensee agrees to indemnify the University and hold the
    University harmless from and against any and all claims, damages and
    liabilities asserted by third parties (including claims for
    negligence) which arise directly or indirectly from the use of the
    Software or the sale of any products based on the Software.

    No part of the Software may be reproduced, modified, transmitted or
    transferred in any form or by any means, electronic or mechanical,
    without the express permission of the University. The permission of
    the University is not required if the said reproduction, modification,
    transmission or transference is done without financial return, the
    conditions of this Licence are imposed upon the receiver of the
    product, and all original and amended source code is included in any
    transmitted product. You may be held legally responsible for any
    copyright infringement that is caused or encouraged by your failure to
    abide by these terms and conditions.

    You are not permitted under this Licence to use this Software
    commercially. Use for which any financial return is received shall be
    defined as commercial use, and includes (1) integration of all or part
    of the source code or the Software into a product for sale or license
    by or on behalf of Licensee to third parties or (2) use of the
    Software or any derivative of it for research with the final aim of
    developing software products for sale or license to a third party or
    (3) use of the Software or any derivative of it for research with the
    final aim of developing non-software products for sale or license to a
    third party, or (4) use of the Software to provide any service to an
    external organisation for which payment is received. If you are
    interested in using the Software commercially, please contact Oxford
    University Innovation ("OUI"), the technology transfer company of the
    University, to negotiate a licence. Contact details are:
    fsl@innovation.ox.ac.uk quoting Reference Project 9564, FSL.*/
#ifndef EXPOSE_TREACHEROUS
#define EXPOSE_TREACHEROUS
#endif

#include "miscmaths/miscmaths.h"
#include "miscmaths/SpMat.h"
#include "streamlines.h"


using namespace MISCMATHS;

void read_file_names(const string& filename,vector<string>& filelist){
  ifstream fs(filename.c_str());
  string tmp;
  if(fs){
    fs>>tmp;
    do{
      filelist.push_back(tmp);
      fs>>tmp;
    }while(!fs.eof());
    }
  else{
    cerr<<filename<<" does not exist"<<endl;
    exit(0);
  }
}

int main(int argc, char** argv){

  if(argc<2){
    cout<<"fdt_matrix_merge <matlist> <outfile> [-omatrix4]"<<endl;
    exit(1);
  }

  vector<string> filelist;
  read_file_names(argv[1],filelist);
  bool domat4 = false;

  if( argc>3 ){ if( string(argv[3])=="-omatrix4"){ domat4=true; } }

  cout<<"merge "<<filelist.size()<<" files"<<endl<<endl;
  if( !domat4 ){
    SpMat<float> omat(filelist[0]);
    SpMat<float>* tmpmat;
    for(unsigned int i=1;i<filelist.size();i++){
      cout<<filelist[i]<<endl;
      cout<<"...load new file"<<endl;
      tmpmat = new SpMat<float>(filelist[i]);
      omat += *tmpmat;
      delete(tmpmat);
   }
    cout<<"save"<<endl;
    omat.Print(argv[2]);
  }
  else{
    cout<<"Matrix4 merge"<<endl;
    string file1=filelist[0]+"1.mtx";
    ifstream fs1(file1.c_str());
    if (!fs1) { 
      cerr << "Could not open file " << file1 << " for reading" << endl;
      return -1;
    }
    int nrows, ncols;
    fs1 >> nrows;
    fs1 >> ncols;
    fs1.close();
    SpMat_HCP omat(nrows,ncols,filelist[0]);
    SpMat_HCP* tmpmat;
      
    //int testcol=15442, testrow=135590;
    for(unsigned int i=1;i<filelist.size();i++){
      cout<<filelist[i]<<endl;
      cout<<"...loading new file"<<endl;
      tmpmat = new SpMat_HCP(nrows,ncols,filelist[i]);
      cout<<"...increment"<<endl;
      //tmpmat->Print(testrow,testcol);
      omat += (*tmpmat);
      //omat.Print(testrow,testcol);
      delete(tmpmat);
    }
    cout<<"save"<<endl;
    omat.SaveTrajFile(argv[2]);
  }
  return 0;
}
