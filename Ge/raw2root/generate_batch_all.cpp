
#define NUM 36

void generate_batch_all()
{
  ofstream file_out;
  file_out.open("batch_all.sh");

  int k=0;
  //50
  for(int i=0;i<6;i++){
	if(k%(NUM)==0 && k>0){
      //file_out << "sleep 1200" << endl;
	  file_out << endl;
	}	  
    file_out << "./batch_single_all.sh 50 " << i << endl;
	k++;
  }

  //51
  for(int i=0;i<30;i++){
	if(k%(NUM)==0 && k>0){
      //file_out << "sleep 1200" << endl;
	  file_out << endl;
	}	  
    file_out << "./batch_single_all.sh 51 " << i << endl;
	k++;
  }

  //52
  for(int i=0;i<101;i++){
	if(k%(NUM)==0 && k>0){
      //file_out << "sleep 1200" << endl;
	  file_out << endl;
	}	  
    file_out << "./batch_single_all.sh 52 " << i << endl;
	k++;
  }

  //53
  for(int i=0;i<65;i++){
	if(k%(NUM)==0 && k>0){
      //file_out << "sleep 1200" << endl;
	  file_out << endl;
	}	  
    file_out << "./batch_single_all.sh 53 " << i << endl;
	k++;
  }

  //54
  for(int i=0;i<55;i++){
	if(k%(NUM)==0 && k>0){
      //file_out << "sleep 1200" << endl;
	  file_out << endl;
	}	  
    file_out << "./batch_single_all.sh 54 " << i << endl;
	k++;
  }

  //55
  for(int i=0;i<53;i++){
	if(k%(NUM)==0 && k>0){
      //file_out << "sleep 1200" << endl;
	  file_out << endl;
	}	  
    file_out << "./batch_single_all.sh 55 " << i << endl;
	k++;
  }

  //56
  for(int i=0;i<120;i++){
	if(k%(NUM)==0 && k>0){
      //file_out << "sleep 1200" << endl;
	  file_out << endl;
	}	  
    file_out << "./batch_single_all.sh 56 " << i << endl;
	k++;
  }

  //57
  for(int i=0;i<37;i++){
	if(k%(NUM)==0 && k>0){
      //file_out << "sleep 1200" << endl;
	  file_out << endl;
	}	  
    file_out << "./batch_single_all.sh 57 " << i << endl;
	k++;
  }

  file_out.close();
}
