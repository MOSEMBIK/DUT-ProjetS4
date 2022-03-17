
#include <Multiplayer/Client.hpp>

using namespace std;



// Constructeur.
Client::Client (const string & host, unsigned short port)
{
	// Mise en place du context
	asio::io_context io_context;
	// Création du socket.
	m_socket = asio::ip::tcp::socket(io_context);
	// Connexion au serveur
	m_socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(host), port));

	for (;;) {
      boost::array<char, 128> buf;
      asio::error_code error;

      size_t len = m_socket.read_some(asio::buffer(buf), error);

	if (error == asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw asio::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
}

Client::~Client ()
{
  // Déconnexion des signaux.
  m_socket.disconnect ();
}

// Commande "#alias"
void Client::process_alias(QTextStream & is)
{
	string id;
	is >> id >> ws;
	emit alias(id);
}

// Commande "#connected"
void Client::process_connected(QTextStream & is)
{
	string id;
	is >> id >> ws;
	emit user_connected(id);
}

// Commande "#disconnected"
void Client::process_disconnected(QTextStream & is)
{
	string id;
	is >> id >> ws;
	emit user_disconnected(id);
}

// Commande "#renamed"
void Client::process_renamed(QTextStream & is)
{
	string oldN, newN;
	is >> oldN >> newN >> ws;
	emit user_renamed(oldN, newN);
}

// Commande "#list"
void Client::process_list(QTextStream & is)
{
	string id;
	stringList list;
	while (!is.atEnd()) {
		is >> id >> ws;
		list.append(id);
	}
	emit user_list(list);
}

// Commande "#private"
void Client::process_private(QTextStream & is)
{
	string id;
	is >> id >> ws;
	emit user_private(id,is.readAll());
}

// Commande "#error"
void Client::process_error (QTextStream & is)
{
  string id;
  is >> id >> ws;
  emit error (id);
}

// Envoi d'un message à travers le socket.
void Client::write (const string & message)
{
  socket.write (message.toUtf8 () + '\n');
}

////////////////////////////////////////////////////////////////////////////////
// ClientWindow //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ClientWindow::ClientWindow (const string & host, unsigned short port, QWidget * parent) :
  QMainWindow (parent),
  Client (host, port, this),
  text (this),
  input (this),
  users (this)
{
  users.setWindowTitle("Liste des utilisateurs");
  text.setReadOnly (true);
  setCentralWidget (&text);

  // Insertion de la zone de saisie.
  // QDockWidget insérable en haut ou en bas, inséré en bas.
  QDockWidget *dock = new QDockWidget(host, parent);
  dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  dock->setWidget(&input);
  this->addDockWidget(Qt::BottomDockWidgetArea,dock);

  // Insertion de la zone des utilisateurs.
  QDockWidget *dockUsers = new QDockWidget(host, parent);
  dockUsers->setAllowedAreas(Qt::RightDockWidgetArea);
  dockUsers->setWidget(&users);
  this->addDockWidget(Qt::RightDockWidgetArea,dockUsers);

  // Désactivation de la zone de saisie.
  input.setEnabled (false);

  // Envoi de messages lorsque la touche "entrée" est pressée.
  // - transmission du texte au moteur de messagerie instantanée ;
  // - effacement de la zone de saisie.
  connect (&input, &QLineEdit::returnPressed, [this] () {
	  Client.write(input.text());
	  input.clear();
  });

  // Connexion.
  // - affichage d'un message confirmant la connexion ;
  // - saisie de l'alias ;
  // - envoi de l'alias ;
  // - activation de la zone de saisie.
  connect ( &Client, &Client::connected, [this] () {
	  string text = QInputDialog::getText(
		this->parentWidget(),
		"Connexion",
		"Entrez votre pseudo"
	  );
	  this->text.setText("<b>Connecté !</b>");
	  Client.write(text);
	  input.setEnabled(true);
  });

  // Déconnexion.
  // - désactivation de la zone de saisie.
  // - affichage d'un message pour signaler la déconnexion.
  connect ( &Client, &Client::disconnected, [this] () {
	  input.setEnabled(false);
	  text.setText("Déconnexion...");
  });

  // Messages.
  connect (&Client, &Client::message, [this] (const string & message) {
	text.append (message);
  });

  // Liste des utilisateurs.
  // Connexion d'un utilisateur.
  // Déconnexion d'un utilisateur.
  // Nouvel alias d'un utilisateur.
  // Message privé.
  connect ( &Client, &Client::user_list, [this] (const stringList & list) {
	  users.clear();
	  string maListe = "Liste des utilisateurs connectés : "+list[0];
	  for (int i=1; i<list.length(); i++) {
		  maListe += ", "+list[i];
		  users.addItem(list[i]);
	  }
	  text.append("<b style='color:blue;'>"+maListe+"</b>");
  });
  connect ( &Client, &Client::user_connected, [this] (const string & name) {
	  text.append("<p style='color:green;'>Connexion de "+name+"</p>");
	  users.addItem(name);
  });
  connect ( &Client, &Client::user_disconnected, [this] (const string & name) {
	  text.append("<p style='color:red;'>Déconnexion de "+name+"</p>");
	  for(int i=0; i < users.count(); i++) {
		  auto item = users.item(i);
		  if (item->text() == name) {
			  delete users.takeItem(i);
		  }
	  }
  });
  connect ( &Client, &Client::alias, [this] (const string & name) {
	  text.append("<p style='color:blue;'>Votre alias est maintenant : "+name+"</p>");
  });
  connect ( &Client, &Client::user_renamed, [this] (const string & oldN, const string & newN) {
	  text.append("<p style='color:blue;'>"+oldN+" s'est renommé en "+newN+"</p>");
  });
  connect ( &Client, &Client::user_private, [this] (const string & name, const string & message) {
	  text.append("<p style='color:gray;'>"+name+" : "+message+"</p>");
  });


  // Gestion des erreurs.
  connect (&Client, &Client::error, [this] (const string & id) {
	QMessageBox::critical (this, tr("Error"), id);
  });

  // CONNEXION !
	text.append (tr("<b>Connecting...</b>"));
}

