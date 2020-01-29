
/**
 * This function will return true if val ∈ [floor, ceil]
 * false otherwise
*/
template <class T>
bool clamp (T val, T floor, T ceil)
{
    return ((val <= ceil) && (val >= floor));
}

// make function to cast time back from seconds fron epoch back to chrono timestamp
// also for other things