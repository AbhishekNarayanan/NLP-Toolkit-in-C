#include <string.h>
#define TRUE 1
#define FALSE 0

static char *b;
static int k;
static int k0;
static int j;


//To check if character at index is a consonant
static int isCons(int index) 
{
  switch (b[index]) 
  {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      return FALSE;
    case 'y':
      return (index == k0) ? TRUE : !isCons(index - 1);
    default:
      return TRUE;
  }
}

//Function to return number of repeated sequences
static int Seqcount() 
{
  int position;
  int index;

  position = 0;
  index = k0;

  while (TRUE) 
  {
    if (index > j) 
      return position;

    if (!isCons(index)) 
      break;  

    index++;
  }

  index++;

  while (TRUE) 
  {
    while (TRUE) 
    {
      if (index > j)
        return position;

      if (isCons(index)) 
        break;
      
      index++;
    }

    index++;
    position++;

    while (TRUE) 
    {
      if (index > j) 
        return position;
      
      if (!isCons(index)) 
        break;
      
      index++;
    }

    index++;
  }
}


//Function returs true when `k0, ... j` contains a vowel. */
static int Isvowel() 
{
  int index;

  index = k0 - 1;

  while (++index <= j) 
  {
    if (!isCons(index)) 
      return TRUE;
  }

  return FALSE;
}

//Function to check for double consonants
static int Isdouble(int index) 
{
  if (b[index] != b[index - 1]) 
  {
    return FALSE;
  }

  return isCons(index);
}

//Function to check for consonant-vowel-consonant sequences
static int cvc(int index) 
{
  int character;

  if (index < k0 + 2 || !isCons(index) || isCons(index - 1) || !isCons(index - 2)) {
    return FALSE;
  }

  character = b[index];

  if (character == 'w' || character == 'x' || character == 'y') {
    return FALSE;
  }

  return TRUE;
}

//To check if the string ends with value
static int endswith(const char *value) 
{
  int length = value[0];

  if (value[length] != b[k]) {
    return FALSE;
  }

  if (length > k - k0 + 1) {
    return FALSE;
  }

  if (memcmp(b + k - length + 1, value + 1, length) != 0) {
    return FALSE;
  }

  j = k - length;

  return TRUE;
}

//Set to value
static void Setto(const char *value) 
{
  int length = value[0];

  memmove(b + j + 1, value + 1, length);

  k = j + length;
}

//Replace a portion of string to value 
static void Replace(const char *value) 
{
  if (Seqcount() > 0) 
    Setto(value);
  
}

//Getting rid of plurals
static void Remplur() 
{
  int character;

  if (b[k] == 's') 
  {
    if (endswith("\04" "sses")) {
      k -= 2;
    } else if (endswith("\03" "ies")) {
      Setto("\01" "i");
    } else if (b[k - 1] != 's') {
      k--;
    }
  }

  if (endswith("\03" "eed")) 
  {
    if (Seqcount() > 0) {
      k--;
    }
  } 
  else if ((endswith("\02" "ed") || endswith("\03" "ing")) && Isvowel()) 
  {
    k = j;

    if (endswith("\02" "at")) {
      Setto("\03" "ate");
    } else if (endswith("\02" "bl")) {
      Setto("\03" "ble");
    } else if (endswith("\02" "iz")) {
      Setto("\03" "ize");
    } else if (Isdouble(k)) {
      k--;

      character = b[k];

      if (character == 'l' || character == 's' || character == 'z') {
        k++;
      }
    } else if (Seqcount() == 1 && cvc(k)) {
      Setto("\01" "e");
    }
  }
}

//Function to change y to i
static void Ytoi() 
{
  if (endswith("\01" "y") && Isvowel()) {
    b[k] = 'i';
  }
}
//Function to map double suffices to single ones(ization (ize+ation) becomes ize)
static void Step2() 
{
  switch (b[k - 1]) 
  {
    case 'a':
      if (endswith("\07" "ational")) {
        Replace("\03" "ate");
        break;
      }

      if (endswith("\06" "tional")) {
        Replace("\04" "tion");
        break;
      }

      break;
    case 'c':
      if (endswith("\04" "enci")) {
        Replace("\04" "ence");
        break;
      }

      if (endswith("\04" "anci")) {
        Replace("\04" "ance");
        break;
      }

      break;
    case 'e':
      if (endswith("\04" "izer")) {
        Replace("\03" "ize");
        break;
      }

      break;
    case 'l':
      if (endswith("\03" "bli")) {
        Replace("\03" "ble");
        break;
      }

      if (endswith("\04" "alli")) {
        Replace("\02" "al");
        break;
      }

      if (endswith("\05" "entli")) {
        Replace("\03" "ent");
        break;
      }

      if (endswith("\03" "eli")) {
        Replace("\01" "e");
        break;
      }

      if (endswith("\05" "ousli")) {
        Replace("\03" "ous");
        break;
      }

      break;
    case 'o':
      if (endswith("\07" "ization")) {
        Replace("\03" "ize");
        break;
      }

      if (endswith("\05" "ation")) {
        Replace("\03" "ate");
        break;
      }

      if (endswith("\04" "ator")) {
        Replace("\03" "ate");
        break;
      }

      break;
    case 's':
      if (endswith("\05" "alism")) {
        Replace("\02" "al");
        break;
      }

      if (endswith("\07" "iveness")) {
        Replace("\03" "ive");
        break;
      }

      if (endswith("\07" "fulness")) {
        Replace("\03" "ful");
        break;
      }

      if (endswith("\07" "ousness")) {
        Replace("\03" "ous");
        break;
      }

      break;
    case 't':
      if (endswith("\05" "aliti")) {
        Replace("\02" "al");
        break;
      }

      if (endswith("\05" "iviti")) {
        Replace("\03" "ive");
        break;
      }

      if (endswith("\06" "biliti")) {
        Replace("\03" "ble");
        break;
      }

      break;
    case 'g':
      if (endswith("\04" "logi")) {
        Replace("\03" "log");
        break;
      }
  }
}

//Function for endings like ic,full,ness etc 
static void Step3() 
{
  switch (b[k]) 
  {
    case 'e':
      if (endswith("\05" "icate")) {
        Replace("\02" "ic");
        break;
      }

      if (endswith("\05" "ative")) {
        Replace("\00" "");
        break;
      }

      if (endswith("\05" "alize")) {
        Replace("\02" "al");
        break;
      }

      break;
    case 'i':
      if (endswith("\05" "iciti")) {
        Replace("\02" "ic");
        break;
      }

      break;
    case 'l':
      if (endswith("\04" "ical")) {
        Replace("\02" "ic");
        break;
      }

      if (endswith("\03" "ful")) {
        Replace("\00" "");
        break;
      }

      break;
    case 's':
      if (endswith("\04" "ness")) {
        Replace("\00" "");
        break;
      }

      break;
  }
}

//Function for endings like -ant,-ence etc
static void step4() 
{
  switch (b[k - 1]) 
  {
    case 'a':
      if (endswith("\02" "al")) {
        break;
      }

      return;
    case 'c':
      if (endswith("\04" "ance")) {
        break;
      }

      if (endswith("\04" "ence")) {
        break;
      }

      return;
    case 'e':
      if (endswith("\02" "er")) {
        break;
      }

      return;
    case 'i':
      if (endswith("\02" "ic")) {
        break;
      }

      return;
    case 'l':
      if (endswith("\04" "able")) {
        break;
      }

      if (endswith("\04" "ible")) {
        break;
      }

      return;
    case 'n':
      if (endswith("\03" "ant")) {
        break;
      }

      if (endswith("\05" "ement")) {
        break;
      }

      if (endswith("\04" "ment")) {
        break;
      }

      if (endswith("\03" "ent")) {
        break;
      }

      return;
    case 'o':
      if (endswith("\03" "ion") && j >= k0 && (b[j] == 's' || b[j] == 't')) {
        break;
      }

      /* takes care of -ous */
      if (endswith("\02" "ou")) {
        break;
      }

      return;
    case 's':
      if (endswith("\03" "ism")) {
        break;
      }

      return;
    case 't':
      if (endswith("\03" "ate")) {
        break;
      }

      if (endswith("\03" "iti")) {
        break;
      }

      return;
    case 'u':
      if (endswith("\03" "ous")) {
        break;
      }

      return;
    case 'v':
      if (endswith("\03" "ive")) {
        break;
      }

      return;
    case 'z':
      if (endswith("\03" "ize")) {
        break;
      }

      return;
    default:
      return;
  }

  if (Seqcount() > 1) {
    k = j;
  }
}

//Function to remove ending e and change ll to l if seq count>1
static void step5() 
{
  int a;

  j = k;

  if (b[k] == 'e') {
    a = Seqcount();

    if (a > 1 || (a == 1 && !cvc(k - 1))) {
      k--;
    }
  }

  if (b[k] == 'l' && Isdouble(k) && Seqcount() > 1) {
    k--;
  }
}


static char* Stem(char *p, int index, int position) 
{
  b = p;
  k = position;
  k0 = index;

  if (k <= k0 + 1) {
    return b; 
  }

  Remplur();

  if (k > k0) {
    Ytoi();
    Step2();
    Step3();
    step4();
    step5();
  }

  b[k+1]='\0';
  return b;
}
