template <class T>
bool clamp (T val, T floor, T ceil)
{
    return ((val <= ceil) && (val >= floor));
}