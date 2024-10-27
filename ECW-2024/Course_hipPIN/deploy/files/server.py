import socket
import subprocess

BANNER = """
    ██╗           █████╗ ██████╗  █████╗  ██████╗██╗  ██╗███████╗    ██╗  ██╗ █████╗  ██████╗██╗  ██╗███████╗             ██╗
   ██╔╝▄ ██╗▄    ██╔══██╗██╔══██╗██╔══██╗██╔════╝██║  ██║██╔════╝    ██║  ██║██╔══██╗██╔════╝██║ ██╔╝██╔════╝    ▄ ██╗▄  ██╔╝
  ██╔╝  ████╗    ███████║██████╔╝███████║██║     ███████║█████╗█████╗███████║███████║██║     █████╔╝ ███████╗     ████╗ ██╔╝ 
 ██╔╝  ▀╚██╔▀    ██╔══██║██╔═══╝ ██╔══██║██║     ██╔══██║██╔══╝╚════╝██╔══██║██╔══██║██║     ██╔═██╗ ╚════██║    ▀╚██╔▀██╔╝  
██╔╝     ╚═╝     ██║  ██║██║     ██║  ██║╚██████╗██║  ██║███████╗    ██║  ██║██║  ██║╚██████╗██║  ██╗███████║      ╚═╝██╔╝   
╚═╝              ╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝    ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝         ╚═╝                                                                                                                              
[+] Init hacking terminal
[+] bip * bip * tiiiic 
[+] Ready to send an APDU !
""" 

def execute_script(buffer):
    try:
        # Exécution du script avec le buffer comme entrée
        java = "/app/jdk1.8.0_401/bin/java -cp '/app/java_classes/:/app/jar/jcardsim-3.0.6.0.jar' ctf.AppletLauncher"
        result = subprocess.run(f"{java} {buffer}", shell=True, capture_output=True, text=True)
        return result.stdout + result.stderr
    except Exception as e:
        return str(e)

def start_server(host='0.0.0.0', port=65432):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print(f"Serveur démarré sur {host}:{port}")
        
        while True:
            conn, addr = s.accept()
            with conn:
                print(f"Connexion de {addr}")
                buffer = b""
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    buffer += data
                
                buffer = buffer.decode().strip()
                print(f"Reçu: {buffer}")
                result = BANNER + "\n" + execute_script(buffer)
                conn.sendall(result.encode())
                print(f"Résultat envoyé: {result}")

if __name__ == "__main__":
    start_server()
