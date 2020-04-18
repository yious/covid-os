unsigned int div_ceil(unsigned int dividend, unsigned int divisior)
{
    return dividend / divisior + ( dividend % divisior != 0);
}

unsigned long long div_ceil_long_long(unsigned long long dividend, unsigned long long divisior)
{
    return dividend / divisior + ( dividend % divisior != 0);
}