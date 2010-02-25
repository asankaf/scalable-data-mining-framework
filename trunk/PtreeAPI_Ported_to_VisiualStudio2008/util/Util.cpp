#include <strstream>
#include <iostream>
#include <vector>
#include <string>
#include "Util.h"
/**
 * Serialize a ptree to an ostream
 */
void serialize(ostream& out, PTree const& pt)throw(){  
    size_type num_bits = pt.size();
    size_type num_blocks = pt.num_blocks();
    //cout<<"Writing Num block: "<<num_blocks<<endl;
    //cout<<"in serialize, num bits: "<<num_bits<<endl;
    //cout<<"in serialize, num blocks: "<<num_blocks<<endl;
    out.write(reinterpret_cast<const char*>(&num_bits), sizeof(size_type));
    out.write(reinterpret_cast<const char*>(&num_blocks), sizeof(size_type));
    vector<unsigned long> data(num_blocks);
    vector<unsigned long>::iterator it=data.begin();
    to_block_range(pt, it);
    //cout<<"First data block:"<<data[0]<<endl;
    out.write(reinterpret_cast<const char*>(&data[0]), data.size()*sizeof(unsigned long));
}       
//

/**
 * deserialize a ptree from an istream
 */
bool deserialize(istream& in, PTree &pt)throw(){
    size_type num_bits;
    size_type num_blocks;
    in.read(reinterpret_cast<char*>(&num_bits), sizeof(size_type));
    //cout<<"in deserialize, num bits: "<<num_bits<<endl;
    in.read(reinterpret_cast<char*>(&num_blocks), sizeof(size_type));
    //cout<<"in deserialize, num blocks: "<<num_blocks<<endl;
    if(in.gcount()!= sizeof(size_type)) return false;
    vector<unsigned long> data(num_blocks);
    in.read(reinterpret_cast<char*>(&data[0]), num_blocks*sizeof(unsigned long));
    
    if(in.gcount()!= num_blocks*sizeof(unsigned long)) return false;
    size_type actual_blocks = num_bits/(sizeof(unsigned long) * CHAR_BIT );
    int extra_bits = num_bits - actual_blocks * sizeof(unsigned long) * CHAR_BIT;
    //cout<<"extra bits: "<<extra_bits<<endl;
    if(extra_bits == 0) pt.append(data.begin(), data.end());
    else{
      pt.append(data.begin(), data.end() - 1);
     // cout<<"in deserialize, before adding extar bits: "<<pt.size()<<endl;
      for(int i = 0; i<extra_bits; i++){
         pt.push_back((data[num_blocks-1] >> i) & 1);
       }
       //cout<<"in deserialize, after adding extar bits: "<<pt.size()<<endl;
    }
    /*if( num_bits < sizeof(unsigned long) * CHAR_BIT ){
       for(int i = 0; i<num_bits; i++){
         pt.push_back((data[0] >> i) & 1);
       }
    }*/
 
    return true;
}


/**
 * Serialize a std::string to an ostream
 */
void str_serialize(ostream& out, const std::string &s)throw(){
  typedef std::string::size_type size_type;
  size_type size  = s.size();
  out.write(reinterpret_cast<const char*>(&size), sizeof(size_type));
  for(size_t i=0; i<size; i++){
     char c = s[i];
     //cout<<"Writing "<<c<<endl;
     out.write(reinterpret_cast<const char*>(&c),sizeof(char));
  }
}

/**
 * deserialize a std::string from an istream
 */
bool str_deserialize(istream& in, std::string & s)throw(){
    s = "";//.clear();
    typedef std::string::size_type size_type;
    size_type  size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_type));
    if(in.gcount()!= sizeof(size_type)) return false;
    
    for(size_t i=0; i<size; i++){
       char c;
       in.read(reinterpret_cast<char*>(&c), sizeof(char));
       if(in.gcount()!= sizeof(char)) return false;
       //cout<<"Reading "<<c<<endl;
       s.push_back(c);
    }
    
    return true;
}

//template <typename T>
/*string to_string(int  value)  
{ 
    string str;
    strstream strm;
    strm << value;
    strm >> str;

    return str;
}

string to_string(double  value)  
{ 
    string str;
    strstream strm;
    strm << value;
    strm >> str;

    return str;
}*/
/**
 * convert a bit_vector to string
 */
string to_string(const bit_vector& v){
 string str = "";
  for (bit_vector::const_iterator i = v.begin(); i < v.end(); ++i){
    if(*i == 1) str += "1";
    else str += "0";
  }
  return str;
}