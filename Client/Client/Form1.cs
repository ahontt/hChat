using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Client
{
    public partial class Form1 : Form
    {

        TcpClient _client;
        byte[] _buffer = new byte[4096];

        public Form1()
        {
            InitializeComponent();

            _client = new TcpClient();
            _client.Connect("127.0.0.1", 55000);

            _client.GetStream().BeginRead(_buffer, 0, _buffer.Length, Server_MessageReceived, null);
        }

        private void Server_MessageReceived(IAsyncResult ar)
        {

        }
    }
}
