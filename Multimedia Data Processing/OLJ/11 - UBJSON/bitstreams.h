#ifndef BITSTREAMS_H
#define BITSTREAMS_H

#include <ostream>

class bitwriter {
	std::ostream& _os;
	unsigned char _buffer;
	int _bits;

	// Aggiunge il bit meno significativo di u al buffer
	void write_bit(unsigned u) {
		_buffer = (_buffer << 1) | (u & 1);
		if (++_bits == 8) {
			_os.put(_buffer);
			_bits = 0;
		}
	}

	// Non si deve né copiare né assegnare!
	bitwriter(const bitwriter&);
	bitwriter& operator= (const bitwriter&);

public:
	bitwriter(std::ostream& os) : _os(os), _bits(0) {}

	// Scrive sul bitstream i count bit meno significativi di u
	void operator() (unsigned u, unsigned count) {
		while (count > 0)
			write_bit(u >> --count);
	}

	// Svuota il buffer sul file
	void flush(unsigned fill_bit = 0) {
		while (_bits > 0)
			write_bit(fill_bit);
	}

	~bitwriter() {
		flush();
	}
};

class bitreader {
	std::istream& _is;
	unsigned char _buffer;
	int _bits;

	// Aggiunge il bit meno significativo di u al buffer
	unsigned read_bit() {
		if (_bits == 0) {
			_is.get(reinterpret_cast<char&>(_buffer));
			_bits = 8;
		}
		return (_buffer >> --_bits) & 1;
	}

	// Non si deve né copiare né assegnare!
	bitreader(const bitreader&);
	bitreader& operator= (const bitreader&);

public:
	bitreader(std::istream& is) : _is(is), _bits(0) {}

	operator std::istream&() { return _is; }

	// Legge dal bitstream i count bit richiesti e li mette nei bit meno significativi del risultato
	unsigned operator() (unsigned count) {
		unsigned u = 0;
		while (count-- > 0)
			u = (u<<1) | read_bit();
		return u;
	}
};

#endif // BITSTREAMS_H