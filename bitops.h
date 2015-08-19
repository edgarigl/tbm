static inline uint32_t bit_mask32(unsigned int bitlen)
{
	return (1ULL << bitlen) - 1;
}

static inline uint32_t bit_field32(uint32_t val,
				unsigned int bit, unsigned int len)
{
	uint32_t r = val >> bit;
	return r & bit_mask32(len);
}
