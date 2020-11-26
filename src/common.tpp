template <typename T>
void common::FIFO(const std::vector<T>& vec, std::vector<std::vector<T>>& vec_stack) {
    vec_stack.insert(vec_stack.begin(), vec);
    if (vec_stack.size() > behav::STACK_SIZE) vec_stack.pop_back();
}
