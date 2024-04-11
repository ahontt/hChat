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
            if (ar.IsCompleted)
            {
                // Receive message
                var BytesIn = _client.GetStream().EndRead(ar);

                if (BytesIn >0)
                {
                    var tmp = new byte[BytesIn];

                    Array.Copy(_buffer, 0, tmp, 0, BytesIn);

                    var str = Encoding.ASCII.GetString(tmp);

                    BeginInvoke((Action)(() => {
                        listBox1.Items.Add(str);
                        listBox1.SelectedIndex = listBox1.Items.Count - 1;
                    }));
                }

                Array.Clear(_buffer, 0, _buffer.Length);
                _client.GetStream().BeginRead(_buffer, 0, _buffer.Length, Server_MessageReceived, null);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var msg = Encoding.ASCII.GetBytes(textBox1.Text);

            _client.GetStream().Write(msg, 0, msg.Length);

            textBox1.Text = "";
            textBox1.Focus();
        }
    }
}
