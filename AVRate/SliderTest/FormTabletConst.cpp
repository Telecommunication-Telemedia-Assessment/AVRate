#include "FormTabletConst.h"
#include "NetworkSlider.h"
#include <string>
#include <msclr\marshal_cppstd.h>

//void FormTabletConst::InitializeTabletConnection(System::Object ^ obj, System::ComponentModel::DoWorkEventArgs ^ args) {
void FormTabletConst::InitializeTabletConnection() {
	
	this->logBox->AppendText("Waiting connection to tablet\r\n");
	network = new NetworkStream(8080);
	this->logBox->AppendText("Connected!\r\n");
	this->logBox->AppendText("Parse configuration file...\r\n");



	SettingsReader^ reader = gcnew SettingsReader(controller->generalSettings->settingsFile);
	// generate the list of sliders/buttons from the XML	
	System::Collections::Generic::List<System::Collections::Generic::List<KeyValueSetting^>^>^ 
		raterList = reader->ReadSliderList();


	System::String^ configurationFile = "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?><root>";

	if(controller->generalSettings->playOnDevice)
		configurationFile += "<playondevice>true</playondevice>";
	else
		configurationFile += "<playondevice>false</playondevice>";

	System::Collections::Generic::List<NetworkSlider^>^ networkSliders = gcnew System::Collections::Generic::List<NetworkSlider^>();

	for each (System::Collections::Generic::List<KeyValueSetting^>^ rater in raterList) {
		RatingGroup^ rg = nullptr;

		NetworkSlider ^slider = gcnew NetworkSlider();
		networkSliders->Add(slider);
		controller->setSlider(slider);


		if (rater[0]->key == "type" && rater[0]->value == "slider") {
			configurationFile += "<slider>";
			bool nameSet = false;
			bool labelSet = false;
			bool minSet = false;
			bool maxSet = false;
			bool showNumberSet = false;

			for each (KeyValueSetting^ kv in rater) {
				if (kv->key == "label") {
					configurationFile += "<label>" + (kv->value) + "</label>";
					labelSet = true;
				}
				else if (kv->key == "name") {
					configurationFile += "<name>" + (kv->value) + "</name>";
					nameSet = true;
				}
				else if (kv->key == "min") {
					configurationFile += "<min>" + (kv->value) + "</min>";
					minSet = true;
				}
				else if (kv->key == "max") {
					configurationFile += "<max>" + (kv->value) + "</max>";
					maxSet = true;
				}
				else if (kv->key == "shownumbers") {
					configurationFile += "<shownumbers>" + (kv->value) + "</shownumbers>";
					showNumberSet = true;
				}
			}

			if(!nameSet) configurationFile += "<name> </name>";
			if(!labelSet) configurationFile += "<label> </label>";
			if(!minSet) configurationFile += "<min>0</min>";
			if(!maxSet) configurationFile += "<min>10</min>";
			if(!showNumberSet) configurationFile += "<shownumbers>true</shownumbers>";
			configurationFile += "</slider>";

		}			
		else if (rater[0]->key == "type" && rater[0]->value == "buttons") {
			bool nameSet = false;

			configurationFile += "<buttons>";
			for each (KeyValueSetting^ kv in rater) {
				if (kv->key == "label") {
					configurationFile += "<label>" + (kv->value) + "</label>";
				}
				else if (kv->key == "name") {
					configurationFile += "<name>" + (kv->value) + "</name>";
				}
			}

			if(!nameSet) configurationFile += "<name> </name>";
			configurationFile += "</buttons>";

		}

	}
	configurationFile += "</root>";

	this->logBox->AppendText("Send configuration file...\r\n");
	
	msclr::interop::marshal_context context;
	network->write(context.marshal_as<std::string>(configurationFile));

	this->logBox->AppendText("Interface ready...\r\n");

	
	for(int i = 0 ; i < controller->GetTotalVideoNumber() ; ++i) {
		std::string line;
		network->read(line);

		if(line == "[ASKPLAYLIST]") {
			this->logBox->AppendText("Tablet is in standalone mode...\r\n");

			System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show("Do you want to send playlists to the tablet?", "Send playlists?", System::Windows::Forms::MessageBoxButtons::YesNo);
			if(result == System::Windows::Forms::DialogResult::Yes) {
				System::Windows::Forms::OpenFileDialog ^openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());;
				openFileDialog1->Filter = L"Playlist files (*.lst)|*.lst|All files (*.*)|*.*";
				openFileDialog1->Title = L"Open Playlist File";
				openFileDialog1->FileName = L"";
				openFileDialog1->Multiselect = true;
				if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					array<System::String ^> ^filenames = openFileDialog1->FileNames;
					network->write("[BEGIN_NEWPLAYLISTS]");
					for(int i = 0 ; i < filenames->Length ; ++i) {
						sendFile(network, filenames[i]);
					}
					network->write("[END_NEWPLAYLISTS]");
				}

			} else {
				network->write("[NOPLAYLIST]");
			}

			network->read(line);
			if(line == "[CLOSE]") {
				controller->quitAppRegular();
			}
		}

		System::String ^arguments;
		for each (NetworkSlider ^sl in networkSliders) {
			arguments += " " + System::Convert::ToString(sl->getSliderValue());
		}
			

		if(!controller->generalSettings->playOnDevice) {			

			if(line == "[PLAY]") {
				this->logBox->AppendText("Received play request!\r\n");

				controller->PlayNext();
				network->write("[RELEASEGUI]");
				this->logBox->AppendText("Wait for rating!\r\n");
			}

		} else {

			if(line == "[NEXT2PLAY]") {
				std::cout << "[NEXT2PLAY]" << std::endl;

				System::String ^nextToPlay = controller->VirtualPlayNext();
				this->logBox->AppendText(nextToPlay + "\r\n");

				network->write(context.marshal_as<std::string>(nextToPlay));
			}
		}

		// read scores from network, now we need to update the network sliders...
		network->read(line);
		this->logBox->AppendText("Receiving scores!\r\n");
		int bgIdx = line.find("]")+1; // the end of [score]
		for(int n = 0 ; n < networkSliders->Count ; ++n) {
			float score = 0;
			int edIdx = line.find(",", bgIdx);
			if(edIdx != -1) {
				System::String^ value = gcnew System::String(line.substr(bgIdx, edIdx-bgIdx).c_str());
				score = (float) Convert::ToDouble(value);
			}
			networkSliders[n]->setValue(score);

			bgIdx = edIdx+1;
		}
		controller->logLastValues();

		this->logBox->AppendText("Scores saved!\r\n");

		if(i < (controller->GetTotalVideoNumber() - 1))
			network->write("[CONTINUE]", 11);
	}
	this->logBox->AppendText("Done!\r\n");
	network->write("[DONE]", 7);
	
	delete network;
	// now you can close the application.
	controller->quitAppRegular();

	
}

void FormTabletConst::sendFile(NetworkStream *network, String ^file) {
	try {
		System::IO::StreamReader ^sr = gcnew System::IO::StreamReader(file);
		String ^file = "|";
		String ^line = sr->ReadLine();
		while(!System::String::IsNullOrEmpty(line)) {
			file += line + "|";
			line = sr->ReadLine();
		}

		file += "[END_ONEPLAYLIST]";

		msclr::interop::marshal_context context;
		network->write(context.marshal_as<std::string>(file));

	} catch (Exception ^) {
		this->logBox->AppendText("[Warning file cannot be open]" + file + "\r\n");
	}
}

