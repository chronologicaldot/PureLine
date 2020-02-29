// Copyright 2017 Nicolaus Anderson
#ifndef LOGIC_H
#define LOGIC_H

namespace logic {

inline bool
And(bool lhs, bool rhs) {
	return (lhs && rhs);
}

inline bool
Or(bool lhs, bool rhs) {
	return (lhs || rhs);
}

inline bool
Xor(bool lhs, bool rhs) {
	return (!lhs != !rhs);
}

inline bool
Nor(bool lhs, bool rhs) {
	return (!lhs || !rhs);
}

inline bool
Nand(bool lhs, bool rhs) {
	return !(lhs && rhs);
}

}

#endif
