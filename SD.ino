int curTimeSec = 0;

struct Lyric {
  int timeSec;

  // 메모리 모자르면 크기 줄여야함
  char lyric[100];
};


File lyricsFile;
File equalizerFile;

struct Lyric nowShowLyric, nextShowLyric;


void playMusic(char c) {
  char str[3];
  
  // 가사 파일은 파일 이름이 '1'이다.
  str[0] = c + 1;
  str[1] = '\0';
  lyricsFile = SD.open(str);
  if (lyricsFile == NULL) {
    return;
  }

  // 가사 파일은 파일 이름이 '1e'이다.
  str[1] = 'e';
  str[2] = '\0';
//  equalizerFile = SD.open(str);

  readLyric(&nowShowLyric.timeSec, nowShowLyric.lyric);
  readLyric(&nextShowLyric.timeSec, nextShowLyric.lyric);
  Serial.println(nowShowLyric.lyric);
  
  isPlaying = true;
  curTimeSec = 0;

//  lyricsFile.close();
}



void readLyric(int *timeSec, char *str) {
  char c;
  int i = 0;

  // 가사를 한 줄 읽어온다.
  while (lyricsFile.available()) {
    c = lyricsFile.read();
    if (c == '\n') {
      str[i] = '\0';

      // 원본 str = [10] abc
      // str = abc
      // timeSec = 10
      *timeSec = parseLyric(str);
      break;
    } else {
      str[i++] = c;
    }
  }
}

// 가사 파일에서 초 수 파싱
int parseLyric(char *str) {
  int i = 1, j = 0;
  char timeSec[4];
  while(str[i] != ']') {
    timeSec[i - 1] = str[i];
    i++;
  }
  timeSec[i - 1] = '\0';

  while(str[++i] != '\0') {
    str[j++] = str[i];
  }
  str[j] = '\0';
  return atoi(timeSec);
}

void sendNextLyric() {
  if (isPlaying) {
    delay(100);
    curTimeSec++;

    // 다음 가사 출력할 시간이 되었다면 다음 가사를 현재 가사로 옮겨오고, 다음 가사를 읽어들인다.
    if (curTimeSec / 10 == nextShowLyric.timeSec - 1) {
      nowShowLyric = nextShowLyric;
      
      readLyric(&nextShowLyric.timeSec, nextShowLyric.lyric);
      Serial.println(nowShowLyric.lyric);
    }
  }
}

