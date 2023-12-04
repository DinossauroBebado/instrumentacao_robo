
#define TRESHOLD 200;

bool mode ; 
bool define_mode(int reading_ground){
    if( reading_ground > 300){
        mode = true;
    }else{
        mode = false;
    }


    return mode ; 
}