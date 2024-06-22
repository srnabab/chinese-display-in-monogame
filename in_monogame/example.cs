public class Game1 : Game
{
    private GraphicsDeviceManager _graphics;
    private SpriteBatch _spriteBatch;
    
    List<Texture2D> Font_Pixel_List;
    private RenderTarget2D _combineTexture2D;
    private RenderTarget2D _cacheTexture2D;
    List<Rectangle> position;
    private Effect outlineEffect;
    private bool Done;
    private int Readed;

    private const float _imageSwitchTime = 0.27f;
    private float _elapsedTime = 0f;
    private int Words_Nums = 0;
    private int Sentences = 0;
    private int []n;
    private int size;

    MouseState mouse_StateNow;
    MouseState mouse_StateBefore;

    public Game1()
    {
        _graphics = new GraphicsDeviceManager(this);
        Content.RootDirectory = "Content";
        IsMouseVisible = true;
    }

    protected override void Initialize()
    {
        // TODO: Add your initialization logic here
        
        Font_Pixel_List = new List<Texture2D>();
        position = new List<Rectangle>();
        _combineTexture2D = new RenderTarget2D(GraphicsDevice, 1000, 600);
        Done = false;
        Readed = 0;

        mouse_StateNow = Mouse.GetState();
        mouse_StateBefore = mouse_StateNow;
        
        base.Initialize();
    }

    protected override void LoadContent()
    {
        _spriteBatch = new SpriteBatch(GraphicsDevice);

        // TODO: use this.Content to load your game content here

        for (int i = 0;i <= 3504;i++)
        {
            Font_Pixel_List.Add(Content.Load<Texture2D>("font/font." + i));
        }
        outlineEffect = Content.Load<Effect>("a");
        
        for (int i = 0;i < 90;i++)
        {
            position.Add(new Rectangle(i % 30 * 32, i / 30 * 32, 32, 32));
        }
    }

    protected override void Update(GameTime gameTime)
    {
        mouse_StateBefore = mouse_StateNow;
        mouse_StateNow = Mouse.GetState();

        
        var key_State = Keyboard.GetState();

        _elapsedTime += gameTime.ElapsedGameTime.Milliseconds;

        if (mouse_StateNow.X >= 0 && mouse_StateNow.Y >= 0 && mouse_StateNow.X <= _graphics.PreferredBackBufferWidth && mouse_StateNow.Y <= _graphics.PreferredBackBufferHeight && IsActive)
        {
            if (_elapsedTime >= _imageSwitchTime)
            {
                _elapsedTime = gameTime.ElapsedGameTime.Milliseconds;
                Words_Nums += 1;
                if (Words_Nums >= 100)
                {
                        Words_Nums = 100;
                }
            }
            if (mouse_StateNow.LeftButton == ButtonState.Pressed && mouse_StateBefore.LeftButton == ButtonState.Released)
            {
                    Words_Nums = 0;
                    Sentences++;
            }
            if (mouse_StateNow.RightButton == ButtonState.Pressed && mouse_StateBefore.RightButton == ButtonState.Released)
            {
                    Words_Nums = 0;
                    Sentences--;
            }
            if (Sentences < 1)
            {
                    Sentences = 1;
            }
            if (Back_Rectangle.Contains(mouse_StateNow.X, mouse_StateNow.Y) && mouse_StateNow.LeftButton == ButtonState.Pressed && mouse_StateBefore.LeftButton == ButtonState.Released)
            {
                    current_Scene = GameScene.MainMenu;
                    Sentences = 0;
            }

        }
        base.Update(gameTime);
    }

    protected override void Draw(GameTime gameTime)
    {
        GraphicsDevice.Clear(Color.White);

        // TODO: Add your drawing code here
        _spriteBatch.Begin();
        if (Sentences == 1)
        {
            if (Words_Nums == 0)
            {
                Done = false;
            }
            if (!Done)
            {
                _spriteBatch.End();
                Text("散文是一种文学形式，与之相对的称为韵文或诗文。散文可分为广义与狭义的解释，其中，广义散文最容易辨识与定义的方式，是“松散”的结构。, Color.Black);
                _spriteBatch.Begin();
            }
            if (Readed < 1)
            {
                Readed = 1;
            }
        }
        if (Sentences == 2)
        {
            if (Words_Nums == 0)
            {
                Done = false;
            }
            if (!Done)
            {
                _spriteBatch.End();
                Text("也就是说，扣除其它文学形式重叠部分，运用普通语法结构，", Color.Black);
                _spriteBatch.Begin();
            }
            if (Readed < 2)
            {
                Readed = 2;
            }
        }
         _spriteBatch.Draw(Background_Texture2D, new Vector2(0, 0), Color.White);

        var once_Color = Color.Black;
                
        if (Sentences < Readed)
        {
            _spriteBatch.Draw(_cacheTexture2D, new Vector2(50, 800), Color.White);
        }
        else
        {
            _spriteBatch.Draw(_cacheTexture2D, new Vector2(50, 800), once_Color);
        }
        _spriteBatch.Draw(Back_Texture2D, Back_Rectangle, Color.White);

        _spriteBatch.End();
        base.Draw(gameTime);
    }

    public void Text(string ch, Color color)
    {
        if (Words_Nums == 0)
        {
            n = F.Font_Num(ch);
            size = F.Strlen();
            F.Free();
        }

        if (Words_Nums > size)
        {
            Words_Nums = size;
            Done = true;
        }

        _spriteBatch.Begin(effect: outlineEffect);
        //_spriteBatch.Begin();
        GraphicsDevice.SetRenderTarget(_combineTexture2D);
        GraphicsDevice.Clear(Color.Transparent);
        for (int s = 0;s < Words_Nums;s++)
        {
            _spriteBatch.Draw(Font_Pixel_List[n[s]], position[s], Color.Black);
        }
        _spriteBatch.End();
        GraphicsDevice.SetRenderTarget(null);
        
        _cacheTexture2D = _combineTexture2D;
    }
}
