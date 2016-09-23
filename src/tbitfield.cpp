// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len<0) throw std::string("negative_length");
	BitLen=len;
	MemLen = ((len-1)/(sizeof(TELEM)*8)+1);
	pMem = new TELEM[MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen = ((BitLen-1)/(32)+1);
	pMem = new TELEM[MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
	delete []pMem;
	pMem=0;
	BitLen=0;
	MemLen=0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return ((n)/(32));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int pos = n-32*GetMemIndex(n); 
	TELEM mask = (1<<(pos));
	return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n<0) throw std::string("negative_bit");
	if (n >= BitLen) throw std::string("too_large_bit_index");
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] = (mask | pMem[index]);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n<0) throw std::string("negative_bit");
	if (n >= BitLen) throw std::string("too_large_bit_index");
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] = (mask ^ pMem[index]);  
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0) throw std::string("negative_bit");
	if (n >= BitLen) throw std::string("too_large_bit_index");
	int index=GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	mask = (mask & pMem[index]);
	if (mask == 0) return 0;
	if (mask != 0) return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen=bf.BitLen;
	if (MemLen!=bf.MemLen) {
		delete []pMem;
		MemLen=bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res=1;
	if (BitLen!=bf.BitLen) res=0;
	else
		for (int i=0; i<MemLen;i++)
			if (pMem[i]!=bf.pMem[i]) {
				res=0;
				break;
			}
	return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int res=0;
	if (BitLen!=bf.BitLen) res=1;
	else
		for (int i=0; i<MemLen;i++)
			if (pMem[i]!=bf.pMem[i]) {
				res=1;
				break;
			}
	return res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len;
	len=BitLen;
	if (BitLen < bf.BitLen) len=bf.BitLen;
	TBitField temp(len);
	for (int i=0; i<MemLen; i++)
		temp.pMem[i]=pMem[i];
	for (int i=0; i<bf.MemLen; i++)
		temp.pMem[i]=(temp.pMem[i] | bf.pMem[i]);

	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len;
	len=BitLen;
	if (BitLen < bf.BitLen) len=bf.BitLen;
	TBitField temp(len);
	for (int i=0; i<MemLen; i++)
		temp.pMem[i]=pMem[i];
	for (int i=0; i<bf.MemLen; i++)
		temp.pMem[i]=(temp.pMem[i] & bf.pMem[i]);

	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for (int i=0; i<BitLen; i++)
		temp.SetBit(i);
	for (int i=0; i<BitLen; i++)
		if (this->GetBit(i)==1) temp.ClrBit(i);
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	return ostr;
}
