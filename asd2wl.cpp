/*
Plik: asd2wl.cpp
Przeznaczenie: Pierwszy projekt laboratoryjny	
	   	przedmiot: ALGORYTMY I STRUKTURY DANYCH

Opracowal:	Mateusz Filipiuk
		mateusz.filipiuk@gmail.com
*/

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "asdparam.h"
#include "asdlab.h"

using namespace std;



struct KV 
{
  Key k;
  Value v;
  KV() { 
    k = Key(); 
    v = Value(); 
  }
  KV(const KV& kv) { k = kv.k; v = kv.v; }
  bool operator==(const KV& kv) 
  { 
    return (k == kv.k) && (v == kv.v); 
  }
  void operator=(const KV& kv) { k = kv.k; v = kv.v; };
} ;

class Dict2WList: public virtual Dict								//Klasa słownika używającego listy dwukierunkowej
{
public:
	Dict2WList();													//Tworzenie pustego słownika
	~Dict2WList();													//Niszczenie słownika
	Dict2WList(const Dict2WList& d);								//Konstruktor kopiujący
	void insert(const Key& k, const Value& val);					//Wstawianie wartosci
	bool exists(const Key& k);										//Sprawdzanie, czy jest
	Value read(const Key& k) const;  								//Odczyt wartosci
	void del(const Key& k);       									//Kasowanie wartosci
	std::string convert() const;									//Slownik w postaci znakowej
	bool info_eq(const Dict2WList& DL);								//Czy informacyjnie ==
	bool struct_eq(const Dict2WList& DL);							//Czy struktura danych ==
	bool operator==(const Dict2WList& DL) { return info_eq(DL); };	//Operacja porownania informacyjnego
	operator const char*() const;
protected:
private:
	struct Node														//Struktura węzła listy dwukierunkowej
	{
		Key k;														//Klucz
		Value v;													//Wartość
		Node *prev, *next;											//Wskaźnik na poprzedni, następny
		Node();														//Konstruktor
		Node(const Key& kee, const Value& val);						//Alternatywny konstruktor
		bool operator== (const Node& n)								//Porównanie zawartości
		{
			return (k == n.k)&&(v == n.v);
		}
		void operator= (const Node& n)
		{
			k = n.k;
			v = n.v;
			prev = n.prev;
			next = n.next;
		}
	};

	Node* find(const Key& k) const;									//Szukanie pierwszego węzła o podanym kluczu
	void unsafeins(const Key& k,const Value& v);							//operacja niebezpiecznego wstawiania

	Node *first;													//Wskaźnik na pierwszy element listy 
	Node *last;														//Wskaźnik na ostatni  element
	int NoElements;													//Ilość elementów (Number of Elements)
};

Dict2WList::Node::Node()											//Implementacja konstruktora struktury Dict2WList::Node
{
	k = 0;
	v = "";
	prev = nullptr;
	next = nullptr;
}

Dict2WList::Node::Node(const Key& kee, const Value& val)			//Implementacja alternatywnego konstruktora Dict2WList::Node
{
	k = kee;
	v = val;
	prev = nullptr;
	next = nullptr;
}

Dict2WList::Dict2WList()											//Implementacja konstruktora klasy Dict2WList
{
	
	first = nullptr;
	last = nullptr;
	NoElements = 0;
}


/* Sortowanie przez wstawianie :) porządek rosnący */
void Dict2WList::insert(const Key& k, const Value& val)				//Implementacja metody Dict2WList::insert
{
	Node *add;
	add = new Node(k,val);						//Tworzę węzeł, który będę dodawał
	
	if (first == nullptr)						//Sprawdzam czy istnieje pierwszy
	{
		first = add;
		last = add;								//Jeśli go nie było to poszło łatwo
	}//if (first == nullptr)
	else										//Jeśli pierwszy jednak istniał
	{
		Node *i;
		i = first;								//Zaczynając od pierwszego
		while (i != nullptr)					//Do momentu aż nie będzie kolejnego elementu
		{
			if (k < i->k)						//Sprawdzam czy klucz wstawianego węzła jest mniejszy od aktualnie wskazywanego
			{
				if (i == first)					//Jeśli już pierwszy węzeł spełniał warunek
				{
					first->prev = add;
					add->next = first;
					first = add;				//To dodawany element staje się nowym pierwszym
					break;						//I przestaję szukać dalej
				} //if (i == first)
				else							//W przeciwnym przypadku
				{
					i->prev->next = add;
					add->next = i;
					add->prev = i->prev;
					i->prev = add;				//Wsuwam gdzieś pomiędzy istniejące węzły
					break;						//I również kończę sprawę
				}//else
			}//if (k < i->k)
			i = i->next;						//Jednak gdy klucz jest większy to przechodzę do kolejnego węzła
		}//while (i != nullptr)					//I powtarzam działanie
		if (i == nullptr)						//W przypadku gdy klucz wstawianego elementu jest większy od wszystkich innych
		{
			add->prev = last;
			last->next = add;
			last = add;							//Wstawiam go na koniec
		}//if (i == nullptr)
	}//else
	++NoElements;								//Liczba elementów zwiększyła się o 1
}

Dict2WList::Node* Dict2WList::find(const Key& k) const					//Implementacja metody Dict2WList::find
{
	Node *i;
	i = first;
	while (i != nullptr)
	{
		if (i->k == k)
			return i;
		else
			i = i->next;
	}//while (i != nullptr)
	return nullptr;
}

bool Dict2WList::exists(const Key& k)									//Implementacja metody Dict2WList::exists
{
	if (this->find(k)!=nullptr)					//Poszukaj elementu o kluczu k
		return true;							//Jeśli znalazł się to zwróć prawdę
	else
		return false;							//a jak nie to fałsz
}

 
Value Dict2WList::read(const Key& k) const  							//Implementacja metody Dict2WList::read

{
	Node *i;									
	i = this->find(k);							//Poszukaj elementu o kluczu k
	if (i != nullptr)							//Jak istnieje
		return i->v;							//To zwróć wartość
	else
		return Value();							//a jak nie to pustą wartość
}

void Dict2WList::del(const Key& k)										//Implementacja metody Dict2WList::del
{
	Node *i;
	i = this->find(k);							//Powiedzmy, że pożądany element się znalazł
	if (i != nullptr)
	{
		if (i->prev == nullptr)					//Gdy nie ma poprzednika to wiem, że jest pierwszym
		{
			first = i->next;					//Więc jego następnik robi się nowym pierwszym
			i->next->prev = nullptr;			//Poprzednik następnika staje się NULLem
			delete i;							//i w tym momencie mogę zapomnieć o elemencie
			return;								//Koniec
		}
		if (i->next == nullptr)
		{
			last = i->prev;
			i->prev->next = nullptr;			
			delete i;
			return;
		}
		i->prev->next = i->next;
		i->next->prev = i->prev;
		delete i;
		return;
	}
	

}

std::string Dict2WList::convert() const
{
	return "a";
}

void Dict2WList::unsafeins(const Key& k, const Value& val)
{

}

Dict2WList::~Dict2WList()
{
}

int main() { 
/*
DictArray DA;
    cout << "start"<< endl; 
    cout << DA.convert();
    DA.insert(1,"test1");
    cout << DA.convert();
    DA.info_eq(DA);
    DA.struct_eq(DA);
    cout << DA.convert();
    DA.del(1);
    cout << DA.convert();
    cout << endl;
    return DA.self_test(1);
	*/
Dict2WList DL;
DL.insert(1,"test1");

	return 0;
}
