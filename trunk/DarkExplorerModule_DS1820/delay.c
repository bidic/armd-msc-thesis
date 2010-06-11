void wait(volatile unsigned int d)
{
  for (;d ;--d);
}

