#define $meow printf ("Poltorashka do meow on %d line in %s\n", __LINE__, __PRETTY_FUNCTION__)
void meowsert (int expresion)
{
    if (expresion == 0)
    {
        $meow;
    }
}
