- [Code Snippets and Utilities](#code-snippets-and-utilities)
  - [Bit Operations](#bit-operations)
    - [Bit Reader](#bit-reader)
    - [Bit Writer](#bit-writer)
    - [Bit Counter](#bit-counter)
  - [Frequency Counter](#frequency-counter)
  - [Huffman](#huffman)
    - [Encoding](#encoding)
    - [Decoding](#decoding)
  - [Matrix](#matrix)

# Code Snippets and Utilities

## Bit Operations

### Bit Reader

```c++
template <typename T>
std::istream &raw_read(std::istream &is, T &val, size_t size = sizeof(T)){
    return is.read(reinterpret_cast<char *> (&val), size);
}

class bitreader {
    std::istream &is_;
    uint8_t buffer_;
    size_t nbits_;
    
    uint32_t read_bit(){
        if(nbits_ == 0){
            raw_read(is_, buffer_);
            nbits_ = 8;
        }
        --nbits_;
        return (buffer_ >> nbits_) & 1;
    }
public:
    bitreader(std::istream &is) : is_(is), nbits_(0) {}
    
	std::istream& read(uint32_t &u, size_t n){
        u = 0;
        while(n --> 0){
            u = (u << 1) | read_bit()
        }
        return is_;
    }
    
    std::istream& read(int32_t &i, size_t n){
        uint32_t u;
        read(u, n);
        i = static_cast<int32_t>(u);
        return is_;
    }
}
```

### Bit Writer

```c++
class bitwriter {
    std::ostream os_;
    uint_8t buffer_;
    size_t nbits_;
    
    std::ostream &write_bit(uint_32t u){
        buffer_ = (buffer << 1) | (u & 1);
        if (++nbits_ == 8){
            raw_write(os_, buffer_);
            nbits_ = 0;
        }
        return os_
    }
    
public:
    bitwriter(std::ostream &os) : os_(os), nbits_(0) {}
    
    ~bitwriter(){
        flush();
    }
    
    std::ostream &write(uint32_t u, size_t n){
        for(size_t i = 0; i < n; ++i){
            write_bit(u >> (n - 1 - i));
        }
        return os_;
    }
    
    void flush(uint_32t u = 0){
        while(n_bits > 0){
            write_bit(u);
        }
    }
    
    std::ostream& operator()(uint32_t u, size_t n){
        return write(u, n);
    }
}

template <typename T>
std::ostream &raw_write(std::ostream &out, const T &val, size_t size = sizeof(T)){
    return os.write(reinterpret_cast<const char *>(&val), size);
}
```

### Bit Counter

```c++
int count_bits(int k){
    int count = 0;
    while(k > 0){
        ++count;
        k /= 2;
    }
    return count;
}
```

## Frequency Counter

```c++
struct frequency_counter {
	std::array<size_t, 256> occurrencies;
	frequency_counter() : occurrencies { 0 } {};

	void operator()(uint8_t val) {
		++occurrencies[val];
	}

	const size_t &operator[](size_t pos) const { return occurrencies[pos]; }

	size_t operator[](size_t pos) { return occurrencies[pos]; }

	double entropy() {
		double tot = 0.0;
		for (const auto &x : occurrencies) {
			tot += x;
		}
		double H = 0.0;
		for (const auto &x : occurrencies) {
			if (x > 0) {
				double px = x / tot;
				H += px * log2(px);
			}
		}
		return H;
	}
};
```

### Generic with Template

``` c++
template<typename symT, typename countT = uin32_t>
struct frequency_counter{
    std::unordered_map<symT, countT> occurrencies_;
    
    void operator(const symT &sym){
        ++occurencies_[symT];
    }
    
    auto operator[](symT &sym){
        return occurrencies_[symT];
    }
    
    auto begin() { return occurencies_.begin(); }
    auto begin() const { return occurencies_.begin(); }
    auto end() { return occurencies_.end(); }
    auto end() const { return occurencies_.end(); }
    
    double entropy(){
        doube tot = 0.0;
        for(const auto &x : occurencies_){
            tot += x.second;
        }
        doubel H = 0.0;
        for(const auto &x : occurrencies_){
            double px = x.second / tot;
            H += px * log2(px);
        }
        return -H;
    }
}
```



## Huffman

```c++
struct HuffNode {
    uint8_t sym_;
    uint32_t freq_;
    
    uint32_t len_;
    uint32_t code_;
    
    HuffNode *left_ = nullptr;
    HuffNode *right_ = nullptr;
    
    HuffNode(uint8_t sym, uint32_t freq) : sym_(sym), freq_(freq) {}
    
    HuffNode(HuffNode *left, HuffNode *right){
        freq_ = left->freq_ + right->freq;
        left_ = left;
        right_ = right;
    }
    
    ~HuffNode(){
        delete left_;
        delete right_;
    }
    
    void generate_codes(std::unordered_map<uint8_t, HuffNode*> &table, uint32_t len = 0, uint32_t code = 0){
        if(left_ == nullptr){
            len_ = len;
            code_ = code;
            table[sym_] = this;
        }
        else {
            left->generate_codes(table, len + 1, (code << 1) | 0);
            right->generate_codes(table, len + 1, (code << 1) | 1);
        }
    }
}
```

### Encoding

```c++
void encode(const std::string &input, const std::string &output){
    // Load File
    std::vector<uint8_t> symbols {std::istream_iterator<uint8_t>(is), std::istream_iterator<uint8_t>()};
    // Compute Frequencies
    std::unordered_map<uint8_t, uint32_t> frequencies;
   	for(const auto &s : symbols){
        ++frequencies[s];
    }
    // Order Nodes in a Vector
    std::vector<HuffNode *> nodes;
    for(const auto &n : frequencies){
        nodes.push_back(new HuffNode(n.first, n.second));
    }
    sort(nodes.begin(), nodes.end(), 
         [](const HuffNode *lhs, const HuffNode *rhs){
             if(lhs->freq_ == rhs->freq_){
                  return lhs->freq_ < rhs->freq_;
             }
             return lhs->freq > rhs->freq_;
         });
    // Create Nodes
    while(nodes.size() > 1){
        // Take the two least probable nodes and remove them from the vector
        auto a = nodes.back();
        nodes.pop_back();
        auto b = nodes.back();
        nodes.pop_back();
        // Combine them in a new Node
        auto c = new HuffNode(a, b);
        auto it = std::lower_bound(nodes.begin(), nodes.end(), c,
        	[](const HuffNode *lhs, const HuffNode *rhs){
            	return lhs->freq_ > rhs->freq_;
            });
        nodes.insert(it, c);
    }
    // Generates Huffman Codes
    auto root = nodes.back();
    nodes.pop_back();
    std::unordered_map(uint8_t, HuffNode) huffman_table;
    root->generate_codes(huffman_table);
    // Canonical Huffman Coding (starts from 1 the most probable)
    std::vector<pair<uint8_t, HuffNode>> canon(huffman_table..begin(), huffman_table.end());
    sort(canon.begin(), canon.sort(), 
    	[](const auto &a, const auto &b){
            if(a.second->freq_ == b.second->freq_){
                return a.first < b.first;
            }
            return a.second->freq_ > b.second->freq_;
        });
    uint32_t len = 1;
    uint32_t code = 0;
    for(auto &c : canon){
        if(c.second->len_ > len){
            code = (code + 1) << (c.second->len_ - len);
            c.second->code = code;
            len = c.second->len_;
        }
        else{
            code++;
            c.second->code_ = code;
        }
    }
}
```

### Decoding

```c++
void decode(std::string &input, std::string &output){
    uint32_t table_size;
    // read table_size
    struct triplet {
        uint32_t sym_;
        uint32_t len_;
        uint32_t code_;
    }
    std::vector<triplet> huff_table;
    for(uint32_t i = 0; i < table_size; ++i){
        triplet t;
        // read triple
        huff_table.push_back(t)
    }
    sort(huff_table.begin(), huff_table.end(), 
        [](const triplet &lhs, const triple &rhs){
            return lhs.len < rhs.len;
        })
    uint32_t size;
    // read size
    // Read Every Symbol
    for(uint32_t i = 0; i < size; ++i){
        uint32_t current_len = 0;
        uint32_t current_code = 0;
        size_t pos = 0;
        // Read the Code 1 bit at time and search it in Huffman Table
        while(true){
            auto current_entry = huff_table[pos];
            while(current_len < current_entry.len_){
                current_code = (current_code << 1) | br.read_bit()
            	current_len++;
            }
            if(current_code == current_entry.code_){
                os.put(current_entry.sym_);
                break;
            }
            ++pos;
            if(pos > huff_table.size){
                error();
            }
        }
    }
}
```

## Matrix

```c++
template<typename T>
class Matrix {
    int rows_, cols_;
    std::vector<T> data_;
    
public:
	Matrix(int rows = 0, int cols = 0)
        : rows_(rows), cols_(cols), data_(rows*cols) {}
    // Operators
    T& operator[](int i) { return data_[i]; }
    const T& operator[](int i) const { return data_[i]; }
    T& operator()(int r, int c) { return data_[r*cols_ + c]; }
    const T& operator()(int r, int c) const { return data_[r*cols_ + c]; }
    
    // Getters
    int rows() const { return rows_; }
    int cols() const { return cols_; }
    int size() const { return rows_*cols_; }
    auto& data() { return data_; }
    const auto& data() const { return data_; }
    
    int rawsize() const { return rows_ * cols_ * sizeof(T); }
    
    auto begin() { return data_.begin(); }
    auto begin() const { return data_.begin(); }
    auto end() { return data_.end(); }
    auto end() const { return data_.end(); }
    
    bool empty() { return rows_ == 0 && cols_ == 0; }
    void resize(int rows, int cols){
        rows_ = rows;
        cols_ = cols;
        data_.resize(rows*cols);
    }
    
    char* rawdata() { return reinterpret_cast<char *>(data_.data()); }
    const char* rawdata() const { return reinterpret_cast<char *>(data_.data()); }
    
}

void flip(matrix<uint8_t> &img){
    for(int r = 0; r < img.rows()/2; ++r){
        for(c = 0; c < img.cols(); ++c){
            std::swap(img(r,c), img(img.rows() - 1 - r, c));
        }
    }
}
```

