#include<bits/stdc++.h>
using namespace std;

const int CHUNK_SIZE = 10;

// Information about the actual chunk:
struct pos_and_size {
  size_t pos;
  size_t size;
};

typedef std::map<size_t, list<pos_and_size>> dupmap;

unsigned int hash_block(vector<char> chunk) {
  unsigned int hash = 0;

  for(int i=0;i<chunk.size();i++){
  	hash+=int(chunk[i]);
  }

  return hash;
}

// Export file helper function
void export_file(dupmap file_map, ifstream &file_in, ofstream &file_out) {

  char data[CHUNK_SIZE];
  int l_chunk_pos = 0;
  int l_chunk_size = 0;

  // Iterate through file map:
  for (const auto it : file_map) {
    list<pos_and_size> l;
    l = it.second;

    bool once = false;

    for (const auto itl : l) {
      l_chunk_pos = itl.pos;
      l_chunk_size = itl.size;
      if (!once) {
        once = true;
        file_out << l.size() << endl;     // How many chunks
        file_out << l_chunk_size << endl; // Current chunk size
        file_in.seekg(l_chunk_pos, file_in.beg);
        file_in.read(data, l_chunk_size); // Save the actual chunk:
        file_out.write(data, l_chunk_size);
      }
      file_out << l_chunk_pos << endl; // Positions located
    }
  }
}
void reduplication(string inpath, string outpath) {

  ifstream file_in(inpath, ios::binary);
  ofstream file_out(outpath, ios::binary);

  if (!file_in) {
    cerr << "error while opening file_in: " << inpath << endl;
    exit(1);
  }

  if (!file_out) {
    cerr << "error while opening file_out: " << inpath << endl;
    exit(1);
  }

  char line[64];
  char data[CHUNK_SIZE];
  size_t count = 0;
  size_t pos = 0;
  size_t size = 0;

  while (file_in) {
    file_in.getline(line,64);
  		// cout<<"line1   "<<line<<endl;
    count = atof(line);
    //cout<<"count "<<count<<endl;
    if (count <= 0) {
      break;
    }
    file_in.getline(line, 64);
  //  cout<<"line2   "<<line<<endl;
    size = atoi(line);
    //cout<<size<<endl;
    memset(data, 0, 64);
    file_in.read(data, size);

    while (count--) {
      file_in.getline(line, 64);
      pos = atoi(line);
     // cout<<"line= "<<line<<" and pos= "<<pos<<endl;
      file_out.seekp(pos, file_in.beg);
      file_out.write(data, size);
    }
  }

 // return true;
}

void deduplication(string inpath,string outpath){
	 ifstream file_in(inpath, ifstream::binary);
  ofstream file_out(outpath, ifstream::binary);

  if (!file_in) {
    cerr << "error while opening file_in: " << inpath << endl;
    exit(1);
  }

  if (!file_out) {
    cerr << "error while opening file_out: " << inpath << endl;
    exit(1);
  }

  file_in.seekg(0, file_in.end);

  dupmap file_map; // hash:count
  size_t total_size = file_in.tellg();
  size_t chunk_size = CHUNK_SIZE;
  size_t total_chunks = total_size / chunk_size;
  size_t last_chunk_size = total_size % chunk_size;
  size_t hash = 0;

/*  cout<<total_size<<endl;
  cout<<total_chunks<<endl;
  cout<<last_chunk_size<<endl;*/

  if (last_chunk_size != 0)
    ++total_chunks;
  else
    last_chunk_size = chunk_size;

/* the loop of chunking */
  file_in.seekg(0, file_in.beg);
  for (size_t chunk = 0; chunk < total_chunks; ++chunk) {

    size_t this_chunk_size =
        chunk == total_chunks - 1 /* if last chunk */
            ? last_chunk_size     /* then fill chunk with remaining bytes */
            : chunk_size;         /* else fill entire chunk */
    size_t start_of_chunk = chunk * chunk_size;
    list<pos_and_size> l;
    pos_and_size pos_and_sz;
    vector<char> chunk_data(this_chunk_size);

    file_in.read(&chunk_data[0], this_chunk_size);

    hash = hash_block(chunk_data);
    l = file_map[hash];

    pos_and_sz.pos = start_of_chunk;
    pos_and_sz.size = this_chunk_size;

    l.push_back(pos_and_sz);
    file_map[hash] = l;
  }

  export_file(file_map, file_in, file_out);
}

int main(){
	deduplication("./file.in","./file.out");
	reduplication("./file.out", "./file.re");
	}