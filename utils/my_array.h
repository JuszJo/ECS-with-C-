#ifndef MY_ARRAY_H 
#define MY_ARRAY_H

template <class T>
class MyArray {
    public:
        MyArray() {
            if (array == NULL) {
                printf("Error: Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
        }

        int size() {
            return array_size - 1;
        }

        void add_element(T element) {
            array[current_index] = element;

            ++current_index;

            if(current_index == array_size) increase_size();
        }

        void remove_element(int index) {
            for(int i = index; i < array_size - 1; ++i) {
                array[i] = array[i + 1];
            }

            --current_index;

            reduce_size();
        }

        // Overload the [] operator for direct array access
        T& operator[](int index) {
            if(index < 0 || index >= current_index) {
                std::cerr << "Error: Index out of bounds." << std::endl;
                std::exit(EXIT_FAILURE);
            }

            return array[index];
        };

    private:
        int array_size = 1;
        int current_index = 0;

        T* array = (T*)malloc(array_size * sizeof(T));

        void increase_size() {
            int new_size = array_size + 1;

            array = (T*)realloc(array, new_size * sizeof(T));

            // Check if realloc succeeded
            if (array == NULL) {
                printf("Error: Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }

            array_size = new_size;
        }

        void reduce_size() {
            int new_size = array_size - 1;

            array = (T*)realloc(array, new_size * sizeof(T));

            // Check if realloc succeeded
            if (array == NULL) {
                printf("Error: Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }

            array_size = new_size;
        }

};

#endif