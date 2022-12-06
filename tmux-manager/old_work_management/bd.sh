#! /bin/bash
cd ..
cp -r m_work_management m_work_management.bd
if [ $? -eq 0 ]; then
echo "backen the file successfully!" 
fi
cd -
