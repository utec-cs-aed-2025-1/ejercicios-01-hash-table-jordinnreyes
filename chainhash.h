#include <vector>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.8;

template<typename TK, typename TV>
struct ChainHashNode {
	TK key;
	TV value;
	size_t hashcode;
	ChainHashNode* next;

	ChainHashNode(TK _key, TV _value, size_t _hashcode) {
		this->key = _key;
		this->value = _value;
		this->hashcode = _hashcode;
		this->next = nullptr;
	}

};

template<typename TK, typename TV>
class ChainHashListIterator {
 	//TODO
	typedef  ChainHashNode<TK,TV> Node;
	Node* current;
public:
	ChainHashListIterator(Node* _current) {
		this->current = _current;

	}
	ChainHashListIterator& operator++() {
		this->current = this->current->next;
		return *this;
	}

	bool operator!=(const ChainHashListIterator& other) {
		return (this->current != other.current);
	}

	Node& operator*() {
		return *this->current;

	}
};

template<typename TK, typename TV>
class ChainHash
{
private:    
    typedef ChainHashNode<TK, TV> Node;
    typedef ChainHashListIterator<TK, TV> Iterator;

	Node** array;  // array de punteros a Node
    int nsize; // total de elementos <key:value> insertados
	int capacity; // tamanio del array
	int *bucket_sizes; // guarda la cantidad de elementos en cada bucket
	int usedBuckets; // cantidad de buckets ocupados (con al menos un elemento)


public:
    ChainHash(int initialCapacity = 10){
		this->capacity = initialCapacity; 
		this->array = new Node*[capacity]();  
		this->bucket_sizes = new int[capacity]();
		this->nsize = 0;
		this->usedBuckets = 0;
	}

	TV get(TK key){
		size_t hashcode = getHashCode(key);
		size_t index = hashcode % capacity;
		
		Node* current = this->array[index];
		while(current != nullptr){
			if(current->key == key) return current->value;
			current = current->next;
		}
		throw std::out_of_range("Key no encontrado");
	}

	int size(){ return this->nsize; }	

	int bucket_count(){ return this->capacity; }

	int bucket_size(int index) { 
		if(index < 0 || index >= this->capacity) throw std::out_of_range("Indice de bucket invalido");
		return this->bucket_sizes[index]; 
	}	
	
	// TODO: implementar los siguientes métodos
	void set(TK key, TV value) {
    	if(fillFactor()> maxFillFactor){rehashing();}
    	size_t hashcode = getHashCode(key);
    	size_t index = hashcode % capacity;

    	//------------------------------------
    	//buscar si cla clase existo, si no crear nuevo nodo e insertar al inicio de la lista

    	Node* head = array[index];
    	for (Node* p = head; p != nullptr; p = p->next) {
    		if (p->key == key) {
    			p->value = value;
    			return;
    		}
    	}

    	Node* newNode = new Node(key,value, hashcode);
    	newNode->next = head;
    	array[index] = newNode;
    	++bucket_sizes[index];
    	++nsize;
    	//--------------------------------
    	if(this->bucket_sizes[index]< 1) {
    		usedBuckets++;
    	}
    	if(this->bucket_sizes[index]> maxColision) {
    		rehashing();
    	}
    }


	bool remove(TK key) {
    	size_t hashcode = getHashCode(key);
    	size_t index = hashcode % capacity;
    	Node* current = this->array[index];
    	Node* prev = nullptr;
    	while (current != nullptr) {
    		if (current->key == key) {
    			// Desenganchar el nodo
    			if (prev == nullptr) {
    				this->array[index] = current->next;
    			} else {
    				prev->next = current->next;
    			}
    			delete current;
    			--nsize;
    			--bucket_sizes[index];
    			if (this->bucket_sizes[index] == 0) {
    				--usedBuckets;
    			}
    			return true;
    		}
    		prev = current;
    		current = current->next;
    	}
    	return false;
    }


	bool contains(TK key) {
    	size_t hashcode = getHashCode(key);
    	size_t index = hashcode % capacity;
    	Node* current = this->array[index];
    	while(current != nullptr) {
    		if (current->Key == key) return true;
    		current = current->next;

    	}
    	return false;
    }


	Iterator begin(int index) {
		return Iterator(this->array[index]);
	}
	Iterator end(int index) {
		return Iterator(nullptr);
	}

private:
	double fillFactor(){
		return (double)this->usedBuckets / (double)this->capacity;
	}	

	size_t getHashCode(TK key){
		std::hash<TK> ptr_hash;
		return ptr_hash(key);
	}

	//TODO: implementar rehashing
	void rehashing() {
		Node** oldArray= this->array;
		int* oldBucket= this->bucket_sizes;
		int oldCapacity = this->capacity;

		this->capacity = this->capacity*2;
		this->array = new Node*[this->capacity]();
		this->bucket_sizes = new int[this->capacity]();
		this->nsize = 0;
		this->usedBuckets=0;


		for (int i = 0; i < oldCapacity; ++i) {
			Node* p = oldArray[i];
			while (p != nullptr) {
				Node* next = p->next;

				size_t newIndex = p->hashcode % this->capacity;


				p->next = this->array[newIndex];
				if (this->array[newIndex] == nullptr) {
					++usedBuckets;
				}
				this->array[newIndex] = p;

				++this->bucket_sizes[newIndex];
				++this->nsize;

				p = next;
			}
		}

		delete[] oldArray;
		delete[] oldBucket;
	}

public:
	// TODO: implementar destructor
	~ChainHash() {
		for (int i = 0; i < this->capacity; ++i) {
			Node* p = this->array[i];
			while (p != nullptr) {
				Node* next = p->next;
				delete p;
				p = next;
			}
		}
		delete[] this->array;
		delete[] this->bucket_sizes;
	}


};




