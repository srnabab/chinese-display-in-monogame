    public void Text(string ch)
    {
        int []n = F.Font_Num(ch);
        int size = F.Strlen();
        int width, height;
        width = height = 0;
        int text_width, text_height;
        text_height = text_width = 50;
        if (Words_Nums > size)
        {
            Words_Nums = size;
        }

        for (int s = 0;s < Words_Nums;s++, width += 50)
        {
            _spriteBatch.Draw(Font_Pixel_List[n[s]], new Rectangle(width, height, text_width, text_height), Color.White);
        }

        F.Free();
    }
