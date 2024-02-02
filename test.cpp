int a = 5;

int getReference() {
   return a;
}

int main() {
   int& b = getReference();
   b = 10;
   return 0;
}