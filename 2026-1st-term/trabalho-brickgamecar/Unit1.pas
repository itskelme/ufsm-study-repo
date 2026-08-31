unit Unit1;

interface

uses
  Winapi.Windows, System.SysUtils, System.Classes,
  Vcl.Controls, Vcl.Forms, Vcl.Grids, Vcl.ExtCtrls,
  Vcl.StdCtrls, Vcl.Graphics, Vcl.Dialogs;

const
  PIXEL_CARRO = 'C';
  PIXEL_OBS   = 'X';
  VAZIO       = ' ';

type
  TForm1 = class(TForm)
    mat        : TStringGrid;
    timer      : TTimer;
    timer_score: TTimer;
    lblPontos  : TLabel;
    procedure FormCreate      (Sender: TObject);
    procedure timerTimer      (Sender: TObject);
    procedure timer_scoreTimer(Sender: TObject);
    procedure FormKeyPress    (Sender: TObject; var Key: Char);
    procedure matDrawCell     (Sender: TObject; ACol, ARow: Integer;
                                Rect: TRect; State: TGridDrawState);
  private
    procedure inicializaObjetos;
    function  noBorder(direcao: Char): Boolean;
    procedure gameOver;
  end;

var
  Form1: TForm1;

  { Variaveis globais do enunciado }
  tamY: Integer = 30;
  tamX: Integer = 60;

  carroLin : Integer;
  carroCol : Integer;
  pontuacao: Integer;

implementation

{$R *.dfm}

{ ---------------------------------------------------------------
  noBorder
--------------------------------------------------------------- }
function TForm1.noBorder(direcao: Char): Boolean;
begin
  Result := False;
  if direcao = 'l' then Result := carroCol - 1 <= 1;
  if direcao = 'r' then Result := carroCol + 1 >= tamX - 2;
end;

{ ---------------------------------------------------------------
  InicializaObjetos  //chamada automaticamente (FormCreate)
  Coloca o carro no centro inferior da tela.
     C
    CCC
     C
    C C
--------------------------------------------------------------- }
procedure TForm1.inicializaObjetos;
var
  i, j: Integer;
begin
  Randomize;

  for i := 0 to tamY - 1 do
    for j := 0 to tamX - 1 do
      mat.Cells[j, i] := VAZIO;

  for i := 0 to tamY - 1 do
  begin
    mat.Cells[0,        i] := '|';
    mat.Cells[tamX - 1, i] := '|';
  end;
  for j := 0 to tamX - 1 do
  begin
    mat.Cells[j, 0]        := '-';
    mat.Cells[j, tamY - 1] := '-';
  end;

  carroLin := tamY - 4;
  carroCol := (tamX div 2) - 1;

  mat.Cells[carroCol,     carroLin - 1] := PIXEL_CARRO;
  mat.Cells[carroCol - 1, carroLin    ] := PIXEL_CARRO;
  mat.Cells[carroCol,     carroLin    ] := PIXEL_CARRO;
  mat.Cells[carroCol + 1, carroLin    ] := PIXEL_CARRO;
  mat.Cells[carroCol,     carroLin + 1] := PIXEL_CARRO;
  mat.Cells[carroCol - 1, carroLin + 2] := PIXEL_CARRO;
  mat.Cells[carroCol + 1, carroLin + 2] := PIXEL_CARRO;

  pontuacao := 0;
end;

{ ---------------------------------------------------------------
  Timer  //chamada automaticamente
  1) Varre a tela procurando obstaculos e os faz descer.
  2) Verifica se o carro esta integro (todos os 7 pixels presentes).
  3) Chama a funcao para gerar outros veiculos.
--------------------------------------------------------------- }
procedure TForm1.timerTimer(Sender: TObject);
var
  i, j, count, col: Integer;
  integro: Boolean;
begin
  count := 0;

  { 1) Varre de baixo para cima }
  for i := tamY - 2 downto 1 do
    for j := 1 to tamX - 2 do
      if mat.Cells[j, i] = PIXEL_OBS then
      begin
        mat.Cells[j, i] := VAZIO;
        if i + 1 < tamY - 1 then
          mat.Cells[j, i + 1] := PIXEL_OBS
        else
          Inc(pontuacao);
        Inc(count);
      end;

  { 2) Verifica integridade do carro (7 pixels) }
  integro :=
    (mat.Cells[carroCol,     carroLin - 1] = PIXEL_CARRO) and
    (mat.Cells[carroCol - 1, carroLin    ] = PIXEL_CARRO) and
    (mat.Cells[carroCol,     carroLin    ] = PIXEL_CARRO) and
    (mat.Cells[carroCol + 1, carroLin    ] = PIXEL_CARRO) and
    (mat.Cells[carroCol,     carroLin + 1] = PIXEL_CARRO) and
    (mat.Cells[carroCol - 1, carroLin + 2] = PIXEL_CARRO) and
    (mat.Cells[carroCol + 1, carroLin + 2] = PIXEL_CARRO);

  if not integro then
  begin
    gameOver;
    Exit;
  end;

  { 3) Gera outros veiculos }
  if Random(4) = 0 then
  begin
    col := 2 + Random(tamX - 4);
    if (mat.Cells[col,     1] = VAZIO) and
       (mat.Cells[col - 1, 2] = VAZIO) and (mat.Cells[col, 2] = VAZIO) and
       (mat.Cells[col + 1, 2] = VAZIO) and
       (mat.Cells[col,     3] = VAZIO) and
       (mat.Cells[col - 1, 4] = VAZIO) and (mat.Cells[col + 1, 4] = VAZIO) then
    begin
      mat.Cells[col,     1] := PIXEL_OBS;
      mat.Cells[col - 1, 2] := PIXEL_OBS;
      mat.Cells[col,     2] := PIXEL_OBS;
      mat.Cells[col + 1, 2] := PIXEL_OBS;
      mat.Cells[col,     3] := PIXEL_OBS;
      mat.Cells[col - 1, 4] := PIXEL_OBS;
      mat.Cells[col + 1, 4] := PIXEL_OBS;
    end;
  end;
end;

{ ---------------------------------------------------------------
  keyPress  //chamada automaticamente
  Move o carro para esquerda (A) ou direita (D).
--------------------------------------------------------------- }
procedure TForm1.FormKeyPress(Sender: TObject; var Key: Char);
var
  i, j: Integer;
begin
  i := carroLin;
  j := carroCol;

  { apaga carro }
  mat.Cells[j,     i - 1] := VAZIO;
  mat.Cells[j - 1, i    ] := VAZIO;
  mat.Cells[j,     i    ] := VAZIO;
  mat.Cells[j + 1, i    ] := VAZIO;
  mat.Cells[j,     i + 1] := VAZIO;
  mat.Cells[j - 1, i + 2] := VAZIO;
  mat.Cells[j + 1, i + 2] := VAZIO;

  if (Key = 'a') or (Key = 'A') then
  begin
    if not noBorder('l') then Dec(carroCol);
  end
  else if (Key = 'd') or (Key = 'D') then
  begin
    if not noBorder('r') then Inc(carroCol);
  end;

  { redesenha carro }
  i := carroLin;
  j := carroCol;
  mat.Cells[j,     i - 1] := PIXEL_CARRO;
  mat.Cells[j - 1, i    ] := PIXEL_CARRO;
  mat.Cells[j,     i    ] := PIXEL_CARRO;
  mat.Cells[j + 1, i    ] := PIXEL_CARRO;
  mat.Cells[j,     i + 1] := PIXEL_CARRO;
  mat.Cells[j - 1, i + 2] := PIXEL_CARRO;
  mat.Cells[j + 1, i + 2] := PIXEL_CARRO;
end;

{ ---------------------------------------------------------------
  gameOver
  timer.Enabled        := false;
  timer_score.Enabled  := False;
  application.MessageBox('Game Over', ':/');
  application.terminate;
--------------------------------------------------------------- }
procedure TForm1.gameOver;
begin
  timer.Enabled       := False;
  timer_score.Enabled := False;
  Application.MessageBox('Game Over', ':/', MB_OK);
  Application.Terminate;
end;

{ ---------------------------------------------------------------
  Pinta celulas do carro em vermelho, obstaculos em cinza escuro
--------------------------------------------------------------- }
procedure TForm1.matDrawCell(Sender: TObject; ACol, ARow: Integer;
  Rect: TRect; State: TGridDrawState);
begin
  if mat.Cells[ACol, ARow] = PIXEL_CARRO then
    mat.Canvas.Brush.Color := clRed
  else if mat.Cells[ACol, ARow] = PIXEL_OBS then
    mat.Canvas.Brush.Color := clDkGray
  else
    mat.Canvas.Brush.Color := clWhite;

  mat.Canvas.FillRect(Rect);
end;

{ ---------------------------------------------------------------
  FormCreate — configura componentes e inicia o jogo
--------------------------------------------------------------- }
procedure TForm1.FormCreate(Sender: TObject);
begin
  KeyPreview := True;

  mat.ColCount         := tamX;
  mat.RowCount         := tamY;
  mat.DefaultColWidth  := 14;
  mat.DefaultRowHeight := 14;
  mat.Options          := mat.Options - [goRangeSelect];
  mat.ScrollBars       := ssNone;
  mat.OnDrawCell       := matDrawCell;

  timer.Interval       := 200;
  timer.Enabled        := True;
  timer_score.Interval := 100;
  timer_score.Enabled  := True;

  inicializaObjetos;
end;

procedure TForm1.timer_scoreTimer(Sender: TObject);
begin
  lblPontos.Caption := 'Pontuacao: ' + IntToStr(pontuacao);
end;

end.
