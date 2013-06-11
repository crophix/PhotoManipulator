#pragma once

namespace PhotoManipulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for PhotoManip
	/// </summary>
	public ref class PhotoManip : public System::Windows::Forms::Form
	{
	public:
		PhotoManip(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PhotoManip()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox;
	private: System::Windows::Forms::Button^  loadImgButton;
	private: System::Windows::Forms::Button^  saveImgButton;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	protected: 


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->loadImgButton = (gcnew System::Windows::Forms::Button());
			this->saveImgButton = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox
			// 
			this->pictureBox->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox->Location = System::Drawing::Point(13, 13);
			this->pictureBox->Name = L"pictureBox";
			this->pictureBox->Size = System::Drawing::Size(723, 648);
			this->pictureBox->TabIndex = 0;
			this->pictureBox->TabStop = false;
			// 
			// loadImgButton
			// 
			this->loadImgButton->Location = System::Drawing::Point(13, 667);
			this->loadImgButton->Name = L"loadImgButton";
			this->loadImgButton->Size = System::Drawing::Size(100, 23);
			this->loadImgButton->TabIndex = 1;
			this->loadImgButton->Text = L"Load";
			this->loadImgButton->UseVisualStyleBackColor = true;
			this->loadImgButton->Click += gcnew System::EventHandler(this, &PhotoManip::loadImgButton_Click);
			// 
			// saveImgButton
			// 
			this->saveImgButton->Location = System::Drawing::Point(13, 696);
			this->saveImgButton->Name = L"saveImgButton";
			this->saveImgButton->Size = System::Drawing::Size(100, 23);
			this->saveImgButton->TabIndex = 2;
			this->saveImgButton->Text = L"Save";
			this->saveImgButton->UseVisualStyleBackColor = true;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &PhotoManip::openFileDialog1_FileOk);
			// 
			// PhotoManip
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(748, 726);
			this->Controls->Add(this->saveImgButton);
			this->Controls->Add(this->loadImgButton);
			this->Controls->Add(this->pictureBox);
			this->Name = L"PhotoManip";
			this->Text = L"PhotoManip";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 }
	private: System::Void loadImgButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "img Files|*.img";
				 openFileDialog1->Title = "Select an image";
				 if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 System::IO::StreamReader ^ sr = gcnew
					 	 System::IO::StreamReader(openFileDialog1->FileName);
					 MessageBox::Show(sr->ReadToEnd());
					 sr->Close();
				 }
   
			 }
	};
}

